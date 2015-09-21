#include <xinu.h>
#include <stdio.h>
static unsigned long	*esp;
static unsigned long	*ebp;

int stackdepth(){
	unsigned long *top_esp, *top_ebp;
	int depth = 0;	
	asm("movl %esp,esp;");
	asm("movl %ebp,ebp;");

	top_esp = esp;
	top_ebp = ebp;
	struct procent	*proc = &proctab[currpid];

	kprintf("------STACKTRACE------\n");
	while(top_esp < (unsigned long *)proc->prstkbase){
		kprintf("Stack Frame\n");
		depth++;
		kprintf("Count: %d\n", depth);
		kprintf("ebp: 0x%08X\n", top_ebp);
		kprintf("esp: 0x%08X\n", top_esp);
		kprintf("size: %10d\n", top_ebp-top_esp);
		top_esp = top_ebp+4;
		top_ebp = (unsigned long *)(*top_ebp);
	}
	kprintf("Stack Base: 0x%08X\n", proc->prstkbase);
	kprintf("Depth: %d\n", depth);

	return depth;
}