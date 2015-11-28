#define MYSIGRECV 5
#define MYSIGALRM 6
#define MYSIGXCPU 7

extern syscall registercbsig(uint16, int ( *func)(void), uint32);