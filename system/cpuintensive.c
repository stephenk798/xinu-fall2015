#include <xinu.h>

#define LOOP1 5
#define LOOP2 234567 

void cpuintensive(){
	int i,j;
	int a = 0;
	for (i=0; i<LOOP1; i++) {
	  for (j=0; j<LOOP2; j++) {
	    a++;
	    a *= 2;
	    a--; 
	  }
	  kprintf("pid: %d, i: %d, priority:%d, preempt: %d\n", currpid, i, proctab[currpid].prprio, preempt);
	}
	kprintf("prcpuused: %d\n", proctab[currpid].prcpuused);
	// Print the CPU time consumed by the process that is recorded in the
	// prcpuused field of the current process's process table entry.
}