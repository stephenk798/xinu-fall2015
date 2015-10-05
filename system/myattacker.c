/* myattacker.c - myattacker */

#include <xinu.h> 

/*------------------------------------------------------------------------
 *  myattacker  -  a process create by main
 *------------------------------------------------------------------------
 */
void myattacker()
{
	kprintf("----Attacker----\n");
	stackdepth();
	return;
}