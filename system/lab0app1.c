/*created 9/7/15*/
/*Prints fun message and its pid*/
#include <xinu.h>

process lab0app1(){
	kprintf("I am a process. My pid is %d", getpid());
	return OK;
}