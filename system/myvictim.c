/* myvictim.c - myvictim */

#include <xinu.h>
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
	sleepms(2000);
	return a;
}
int afunc(){
	int a = 9;
	int b = 0;
	b = a+subsleep();
	
	return a;
}
void myvictim()
{
	afunc();
	kprintf("myvictimglobal: %d\n", myvictimglobal);  
	return;
}