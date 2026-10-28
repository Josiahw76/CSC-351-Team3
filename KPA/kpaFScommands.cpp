#include "kpaFScommands.h"
#include <ctime>
#include <queue>

//******************************************************************************

// Author: Simeon K

bool parseCreateArgs(string requestArgs, string &name){
    name.clear();

    //pwd is seperate and these arguments contain the file name.
    if(requestArgs.empty() || requestArgs.find_first_not_of(' ') == string::npos) {
        return false;
    }

    // Leave room for the ending '\0' in the direcctory entry.
    if (requestArgs.size() >= fnameMaxSize) {
        return false;
    }

    name = requestArgs;
    return true;
}

//******************************************************************************

// Author: Simeon K
// requires node.gen to have been initialized or restored.
void setCreationMetadata(iNode &node, fileType type, uint16_t ownerUID, uint16_t ownerGID, uint16_t permissions) {
    time_t now = time(NULL); // Get the current time.
    // Set the iNode's type, owner UID and GID, and permissions.
    node.type = type;
    node.uid = ownerUID;
    node.gid = ownerGID;
    node.perms = permissions;

    // Directories start with two links and files start with one link.
    if (type == DIR) {
        node.nLinks = 2;
    } else {
        node.nLinks = 1;
    }

    // A newly created file has no data, so its size is zero.
    node.size = 0;
    node.mTime = now;
    node.aTime = now;
    node.bTime = now;
    node.cTime = now;

    // increment the generation count.
    node.gen++;
}

//******************************************************************************

// Author: Simeon K
string formatPermissions(uint16_t perms) {
    string result = "---------";

    // Owner permissions
    if ((perms & 0400) != 0) result[0] = 'r';
    if ((perms & 0200) != 0) result [1] = 'w';
    if ((perms & 0100) != 0) result[2] = 'x';

    // Group permissions.
    if ((perms & 0040) != 0) result[3] = 'r';
    if ((perms & 0020) != 0) result[4] = 'w';
    if ((perms & 0010) != 0) result[5] = 'x';

    // Permissions for everyone else
    if ((perms & 0004) != 0) result[6] = 'r';
    if ((perms & 0002) != 0) result[7] = 'w';
    if ((perms & 0001) != 0) result[8] = 'x';

    return result;
}

//******************************************************************************

// Author: Simeon K
int findDirectoryEntry(const dirEntryType entries[], unsigned int entryCount, string name) {

    // Check each filename in the supplied directory entries
    for (unsigned int i = 0; i < entryCount; i++) {
        if (name == entries[i].fname) {
            return i;
        }
    }

    return -1; // No matching filename was found
}

int findINodeInDirectory(const iNode &directory, const db blockPool [], string name) {
    unsigned int entriesLeft = directory.size / sizeof(dirEntryType);

    // An empty directory has no entries or data blocks to search.
    if (entriesLeft == 0) {
        return -1;
    }

    const indexType *firstIndex = blockPool[directory.addr].index;

    for (unsigned int i = 0; i < indexPerBlock && entriesLeft > 0; i++) {
        const indexType *secondIndex = blockPool[firstIndex[i]].index;

        for (unsigned int j = 0; j < indexPerBlock && entriesLeft > 0; j++) {
            const dirEntryType *entries = blockPool[secondIndex[j]].dirEntry;

            // The final block may contain fewer entries;
            unsigned int count = entriesLeft;
            if (count > dirEntryPerBlock) {
                count = dirEntryPerBlock;
            }

            int position = findDirectoryEntry(entries, count, name);
            if (position != -1) {
                return entries[position].iNodeIndex;
            }

            entriesLeft -= count;
        }
    }

    return -1;
}

int findDirectoryByPath(string path, const iNode iNodePool[], const db blockPool[]) {
    if (path.empty() || path[0] != '/' || iNodePool[0].type != DIR) {
        return -1;
    }

    // Store directory names in the order they appear in the path.
    queue<string> names;
    string::size_type start = 1;

    while (start < path.size()) {
        string::size_type end = path.find('/', start);
        if (end == string::npos) {
            end = path.size();
        }

        if (end > start) {
            names.push(path.substr(start, end - start));
        }
        start = end + 1;
    }

    // Every absolute path starts at the inode root.
    int current = 0;

    while (!names.empty()) {
        int next = findINodeInDirectory(iNodePool[current], blockPool, names.front());

        if (next == -1 || iNodePool [next].type != DIR) {
            return -1;
        }

        current = next;
        names.pop();
    }

    return current;

}

string my_chdir(string path, string pwd) {
    // Invalid path replies keep the shell's existing pwd
    string failure(1, -3);
    failure += ' ';
    failure += pwd;

    if (path.empty()) {
        return failure;
    }

    // Absolute paths start at root; relative paths start at pwd
    int current = 0;
    string target = "/";

    if (path[0] != '/') {
        target = pwd;
        while (target.size() > 1 && target.back() == '/') {
        target.pop_back();
    }

    current = findDirectoryByPath(target, iNodePool, blockPool);
}

if (current == -1 || iNodePool[current].type != DIR) {
    return failure;
}

if (path == "..") {
    string::size_type slash = target.find_last_of('/');
    target = target.substr(0, slash);
    if (target.empty()) {
        target = "/";
    }
} else {
    // Split the requested path into directory names
    queue<string> names;
    string::size_type start = 0;

    while (start < path.size()) {
        string::size_type end = path.find ('/', start);
        if (end == string::npos) {
            end = path.size();
        }

        if (end > start) {
            names.push(path.substr(start, end - start));
        }
        start = end + 1;
    }

    // Search from the selected starting iNode
    while (!names.empty()) {
        string name = names.front();
        names.pop();

        int next = findINodeInDirectory(iNodePool[current], blockPool, name);

        if (next == -1 || iNodePool[next].type != DIR) {
            return failure;
        }

        current = next;
        if (target != "/") {
            target += '/';
        }
        target += name;
    }
}

string reply(1, 1);
reply += ' ';
reply += target;
return reply;
}