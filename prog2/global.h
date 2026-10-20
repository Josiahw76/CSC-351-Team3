#ifndef __GLOBAL_H
#define __GLOBAL_H

// instructions are:
// S value - set program's value to provided value
// A value - add provided value to program's value
// D value -  deduct provided value from program's value
// F offset - fork the process and create a duplicate process with the current
// process as the parent (ppid = current pid). Set the new process's PC to
// the PC of the current process + 1. set the process's values accordingly:
// starting priority is 0
// time used is 0
// start time is current clock value
// block time is 0
// value is value of parent process's value
// new process gets enqueued to ReadyState queue
//
// B value - block current process from running by "value" number of clock ticks
// essentially interpret value as clock ticks and block for value ticks


#endif
