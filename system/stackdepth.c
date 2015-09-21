#include<xinu.h>
#include<stdio.h>
int stackdepth(){
	unsigned long *top_esp, *top_ebp;
	pid = getpid();
	struct procent	*proc = &proctab[pid];
	kprintf("prstkbase: 0x%08X\n", proc->prstkbase);

	return 0;
}