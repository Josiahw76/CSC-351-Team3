#include "common.h"
#include <iostream>

using namespace std;

uint8_t uid, gid;

int main() {
	uid = gid = 0;
	kpaTCP *tcp = new kpaTCP("127.0.0.1", 8080, true);
	tcp->commsListen();

	int bytes;
	char buf[1024];
	char rc[1];
	rc[0] = static_cast<char>(-1);
	while (true) {
		cout << "> ";
		bytes = tcp->commsRead(buf, sizeof(buf));

		if (bytes > 0) {
			if (buf[0] == '~') {
				exit(1);
			}
			cout << buf << endl;
			tcp->commsWrite(rc, 1);

		} else {
			break;
		}

	}
	return 1;
}
