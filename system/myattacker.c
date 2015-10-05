/* myattacker.c - myattacker */

#include <xinu.h> 

/*------------------------------------------------------------------------
 *  myattacker  -  a process create by main
 *------------------------------------------------------------------------
 */
void myattacker()
{
	kprintf("----Attacker----\n");
	unsigned long *top_esp, *top_ebp;

	int depth = 0;	

	asm("movl %esp,top_esp;"); //get the esp and ebp into the global vars
	asm("movl %ebp,top_ebp;");

	// top_esp = esp; //and set local vars to the global ones
	// top_ebp = ebp;
	kprintf(" ebp: 0x%08X\n", top_ebp);
	kprintf(" esp: 0x%08X\n", top_esp);
	// stackdepth();
	return;
}