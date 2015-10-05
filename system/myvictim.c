/* myvictim.c - myvictim */

#include <xinu.h>

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
	kprintf("return address: 0x%08X\n", __builtin_return_address(0));
	sleepms(2000);
	return a;
}
void myvictim()
{
	kprintf("----Victim----\n");
	stackdepth();
	addnumbers();
	subsleep();
	kprintf("myvictimglobal: %d\n", myvictimglobal);  
	return;
}