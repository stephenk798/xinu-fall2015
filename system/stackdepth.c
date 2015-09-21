#include <xinu.h>
#include <stdio.h>
static unsigned long	*esp;
static unsigned long	*ebp;

int stackdepth(){
	unsigned long *top_esp, *top_ebp;
	top_esp = top_ebp-1;
	
	// asm("movl %esp, esp;");
	// asm("movl %ebp, ebp;");
	// kprintf("top_esp: 0x%08X\n", top_esp);
	// kprintf("top_ebp: 0x%08X\n", top_ebp);
	kprintf("------STACKTRACE------\n");
	stacktrace(currpid);
	struct procent	*proc = &proctab[currpid];
	if(top_esp == (unsigned long *)proc->prstkbase){
		kprintf("Stack Base: 0x%08X\n", proc->prstkbase);

	}

	return 0;
}