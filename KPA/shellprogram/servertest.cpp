#include "kpaTCP.h"
#include <iostream>

using namespace std;

int main() {
	kpaTCP *tcp = new kpaTCP("127.0.0.1", 8080, true);
	tcp->commsListen();

	int bytes;
	char buf[1024];
	while (true) {
		cout << "> ";
		bytes = tcp->commsRead(buf, sizeof(buf));

		if (bytes > 0) {
			if (buf[0] == '~') {
				exit(1);
			}
			cout << buf << endl;
		}
	}
	return 1;
}
