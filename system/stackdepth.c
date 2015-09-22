#include <xinu.h>
#include <stdio.h>
static unsigned long	*esp;
static unsigned long	*ebp;

int stackdepth(){
	unsigned long *top_esp, *top_ebp;

	int depth = 0;	

	asm("movl %esp,esp;"); //get the esp and ebp into the global vars
	asm("movl %ebp,ebp;");

	top_esp = esp; //and set local vars to the global ones
	top_ebp = ebp;
	struct procent	*proc = &proctab[currpid]; //get the stack base

	kprintf("------STACKDEPTH------\n");
	while(top_esp < (unsigned long *)proc->prstkbase){ //print stack frames until the top esp >= stack base
		//increment depth and then print relevant data
		depth++;
		kprintf("Stack Frame\n");
		kprintf(" Count: %d\n", depth);
		kprintf(" ebp: 0x%08X\n", top_ebp);
		kprintf(" esp: 0x%08X\n", top_esp);
		kprintf(" size: %10d\n", (uint32)(top_ebp-top_esp);

		top_esp = top_ebp+4; //set esp to the bottom of the next stack frame
		top_ebp = (unsigned long *)(*top_ebp); //set ebp to the address that is stored in itself
	}
	kprintf("Stack Base: 0x%08X\n", proc->prstkbase); //print the stack base when complete
	return depth;
}