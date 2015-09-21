#include<xinu.h>
#include<stdio.h>
int stackdepth(){
	unsigned long *top_esp, *top_ebp;

	asm("mov %%esp, %0;"
		 "mov %%ebp, %1;"
		:"=r"(top_esp), "=r" (top_ebp));
	kprintf("top_esp: 0x%08X\n", top_esp);
	kprintf("top_ebp: 0x%08X\n", top_ebp);
	struct procent	*proc = &proctab[currpid];
	if(top_esp == proc->prstkbase){
		kprintf("Stack Base: 0x%08X\n", proc->prstkbase);

	}

	return 0;
}