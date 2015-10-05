/* myvictim.c - myvictim */

#include <xinu.h>
static unsigned long	*esp;
static unsigned long	*ebp;
/*------------------------------------------------------------------------
 *  myvictim  -  a process create by main
 *------------------------------------------------------------------------
 */

int addnumbers()
{
	int x = 2;
	int y = 5;
	int z;
	z = x+y;
	z = z * x;
	return z;
}
int subsleep(){
	int a = 45;
	int b = 42;
	int c;
	c = a - b;
	a = a - c;
	unsigned long *top_esp, *top_ebp;

	int depth = 0;	
	kprintf("----victim func----\n");
	asm("movl %esp,esp;"); //get the esp and ebp into the global vars
	asm("movl %ebp,ebp;");
	top_esp = esp;
	top_ebp = ebp;
	// top_esp = esp; //and set local vars to the global ones
	// top_ebp = ebp;
	kprintf(" ebp: 0x%08X ebpVal: 0x%08X\n", top_ebp, *top_ebp);
	kprintf(" esp: 0x%08X espVal: 0x%08X\n", top_esp, *top_esp);
	sleepms(2000);
	return a;
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
	addnumbers();
	subsleep();
	kprintf("myvictimglobal: %d\n", myvictimglobal);  
	return;
}