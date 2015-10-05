/* myvictim.c - myvictim */

#include <xinu.h>
static unsigned long	*esp;
static unsigned long	*ebp;
/*------------------------------------------------------------------------
 *  myvictim  -  a process create by main
 *------------------------------------------------------------------------
 */

int subsleep(){
	int a = 45;
	int b = 42;
	int c;
	c = a - b;
	a = a - c;
	unsigned long *top_esp, *top_ebp;
	//ITS EBP+1 THATS WHERE RETURN ADDRESS IS STORED
	kprintf("subsleep return: 0x%08X\n",__builtin_return_address(0));
	kprintf("----victim func----\n");
	asm("movl %esp,esp;"); //get the esp and ebp into the global vars
	asm("movl %ebp,ebp;");
	top_esp = esp;
	top_ebp = ebp;
	// top_esp = esp; //and set local vars to the global ones
	// top_ebp = ebp;
	kprintf(" ebp: 0x%08X ebpVal: 0x%08X\n", top_ebp, *top_ebp);
	kprintf(" ebp+1: 0x%08X ebpVal: 0x%08X\n", (top_ebp+1), *(top_ebp+1));
	kprintf(" esp: 0x%08X espVal: 0x%08X\n", top_esp, *top_esp);
	
	sleepms(2000);
	return a;
}
int afunc(){
	int a = 9;
	return a+subsleep;
}
void myvictim()
{
	unsigned long *top_esp, *top_ebp;
	kprintf("----Victim----\n");
	//stackdepth();
	asm("movl %esp,esp;"); //get the esp and ebp into the global vars
	asm("movl %ebp,ebp;");
	top_esp = esp;
	top_ebp = ebp;
	// top_esp = esp; //and set local vars to the global ones
	// top_ebp = ebp;
	kprintf(" ebp: 0x%08X\n", top_ebp);
	kprintf(" esp: 0x%08X\n", top_esp);
	afunc();
	kprintf("vicpid: %d myvictimglobal: %d\n", currpid, myvictimglobal);  
	return;
}