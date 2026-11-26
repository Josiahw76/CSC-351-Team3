#!/usr/bin/env bash
# Run from the repo root: bash KPA/stat-smoke.sh
# This exercises stat / against an in-memory root using the team's TCP class.
# It builds a known checkpoint so it can run while the normal server is being fixed.
set -euo pipefail
repo_dir=$(git rev-parse --show-toplevel)
checkpoint=9826c10
if ! git -C "$repo_dir" cat-file -e "$checkpoint^{commit}" 2>/dev/null; then
    printf '%s\n' 'The test checkpoint is missing. Use a full clone of the team repository.' >&2
    exit 1
fi

smoke_dir=$(mktemp -d)
git -C "$repo_dir" archive "$checkpoint" KPA | tar -xf - -C "$smoke_dir"
cd "$smoke_dir/KPA"
test_port=$(python3 -c 'import socket; s = socket.socket(); s.bind(("127.0.0.1", 0)); print(s.getsockname()[1])')
export test_port

# Build the server with the team's TCP and stat code, listening on loopback.
cat > stat-server.cpp <<'CPP'
#include "common.h"
#include "kpaFScommands.h"
#include <cstdio>

unsigned int uid = 0, gid = 0;
iNodeType root = {};
iNodeType *iNodePool = &root;
db *blockPool = nullptr;

int main(int argc, char *argv[]) {
    if (argc != 2) { return 1; }
    setCreationMetadata(root, DIR, 0, 0, 0777);

    kpaTCP server("127.0.0.1", stoi(argv[1]), true);
    server.commsListen();

    // Receive one NUL-terminated request from the shell.
    string request;
    char byte;
    bool complete = false;
    while (request.size() < 4096 && server.commsRead(&byte, 1) == 1) {
        if (byte == '\0') {
            complete = true;
            break;
        }
        request += byte;
    }

    // This checkpoint has only root inode 0.
    string reply(1, -3);
    string rootRequest(1, static_cast<char>(_STAT));
    rootRequest += " 0:0 /";
    if (complete && request == rootRequest) {
        reply = string(1, 2) + ' ' + my_stat(0, "/");
    }

    // Include the terminator so the shell knows where the reply ends.
    unsigned int replySize = reply.size() + 1;
    return server.commsWrite(reply.data(), replySize) == static_cast<int>(replySize) ? 0 : 1;
}
CPP

# Make the smoke server local-only and frame requests/replies in a shell copy.
python3 - <<'PY'
from pathlib import Path
import os

def replace(path, old, new):
    source = path.read_text()
    if source.count(old) != 1:
        raise SystemExit(f"{path}: expected local checkpoint code was not found: {old!r}")
    path.write_text(source.replace(old, new, 1))

replace(Path("common.cpp"), "serverAddress.sin_addr.s_addr = INADDR_ANY;",
        "serverAddress.sin_addr.s_addr = inet_addr(serverIPaddress.c_str());")
replace(Path("shellprogram/shellmain.cpp"), "#define DEFAULT_PORT 8080",
        "#define DEFAULT_PORT " + os.environ["test_port"])
shell = Path("shellprogram/shell.cpp")
replace(shell, "msg += pwd;", "if (!pwd.empty()) { msg += ' ' + pwd; }")
replace(shell, "\tchar rc[4096];\n", "")
s = shell.read_text()
start = s.index("\t\tchar *str = (char *)msg.c_str();")
end = s.index("\t\trc_str = rc;", start) + len("\t\trc_str = rc;")
s = s[:start] + """        // Send the entire message and its terminator.
        unsigned int size = msg.size() + 1;
        if (tcpSocket->commsWrite(msg.data(), size) != static_cast<int>(size)) {
            return "";
        }
        char byte;
        while (rc_str.size() < 4096 && tcpSocket->commsRead(&byte, 1) == 1) {
            if (byte == '\\0') {
                if (!rc_str.empty()) { checkCode(rc_str); }
                return rc_str;
            }
            rc_str += byte;
        }
        return "";""" + s[end:]
shell.write_text(s)
s = shell.read_text()
start = s.index("void stat() {")
old = "if ((uint8_t)rc[0] > 0) {"
assert old in s[start:]
shell.write_text(s[:start] + s[start:].replace(old,
    "if (!rc.empty() && (rc[0] == 1 || rc[0] == 2)) {", 1))
replace(Path("shellprogram/common.cpp"), "\t\tcase 1:\n", "\t\tcase 1:\n\t\tcase 2:\n")
PY

g++ -std=c++17 -Wall -Wextra -Wno-unused-parameter \
    stat-server.cpp common.cpp kpaFScommands.cpp fdTable.cpp -o stat-server
g++ -std=c++17 -Wall -Wextra -Wno-unused-parameter \
    shellprogram/shellmain.cpp shellprogram/common.cpp shellprogram/shell.cpp -o shell

./stat-server "$test_port" > server.log 2>&1 &
server_pid=$!
trap 'if kill -0 "$server_pid" 2>/dev/null; then kill "$server_pid"; fi' EXIT
for i in {1..30}; do
    if grep -q 'FS listening on port' server.log; then break; fi
    if ! kill -0 "$server_pid" 2>/dev/null; then cat server.log; exit 1; fi
    sleep 0.1
done
grep -q 'FS listening on port' server.log || { cat server.log; exit 1; }
# The current shell returns 1 after a normal exit.
printf 'stat /\nexit\n' | ./shell | tee shell.log || test "$?" -eq 1
wait "$server_pid"
grep -Eq '^[-rwx]{9} 2 0 0 0 [0-9]+ 0 /$' shell.log
printf 'stat / passed over TCP. Test files: %s\n' "$smoke_dir"
