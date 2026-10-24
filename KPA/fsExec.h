#ifndef __FSEXEC_H
#define __FSEXEC_H

#include "shellprogram/kpaTCP.h"
#include <string>

//*****************************************************************************
// 				DEFINITIONS
//*****************************************************************************

enum SYS_CALL {
	CREAT,
	OPEN,
	CLOSE,
	READ,
	WRITE,
	LSEEK,
	STAT,
	FSTAT,
	MKDIR,
	RMDIR,
	LINK,
	UNLINK,
	CHDIR,
	OPENDIR,
	CLOSEDIR,
	READDIR,
	REWINDDIR
};

kpaTCP *tcpSocket;





#endif
