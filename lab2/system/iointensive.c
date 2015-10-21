#include <xinu.h>

#define LOOP1 5
#define LOOP2 12 

void iointensive(){
	int i,j;
	int a = 0;
	for (i=0; i<LOOP1; i++) {
	  for (j=0; j<LOOP2; j++) {
	    sleepms(2);
	  }
	  kprintf("\npid: %d, i: %d, priority:%d, preempt: %d\n", currpid, i, proctab[currpid].prprio, preempt);
	}
	kprintf("\npid: %d prcpuused: %d\n", currpid, proctab[currpid].prcpuused);
	// Print the CPU time consumed by the process that is recorded in the
	// prcpuused field of the current process's process table entry.
}