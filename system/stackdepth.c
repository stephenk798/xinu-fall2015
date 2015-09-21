#include<xinu.h>
#include<stdio.h>
int stackdepth(){
	unsigned long *top_esp, *top_ebp;
	unsigned long *cur_esp, *cur_ebp;

	asm("mov top_esp, esp");
	asm("mov top_ebp, ebp");
	asm("mov cur_esp, esp");
	kprintf("cur_esp: 0x%08X\n", cur_esp);
	int pid = getpid();
	struct procent	*proc = &proctab[pid];
	if(top_esp == proc->prstkbase){
		kprintf("Stack Base: 0x%08X\n", proc->prstkbase);

	}

	return 0;
}