/* myattacker.c - myattacker */

#include <xinu.h> 
static unsigned long	*ebp;
/*------------------------------------------------------------------------
 *  myattacker  -  a process create by main
 *------------------------------------------------------------------------
 */
void myattacker()
{
	unsigned long *return_add; //the return address of victim

	asm("movl %ebp,ebp;");
		kprintf(" ebp: 0x%08X ebpVal: 0x%08X\n", ebp, *ebp);

	return_add = ebp+505;
	*return_add = &myattackermalware;
	return;
}