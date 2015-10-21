/*created 9/7/15*/
/*Calculates 3782 times 751, prints it and its pid*/
#include <xinu.h>

process lab0app2(){
	int result = 3782*751;
	kprintf("3782 times 751 equals %d. My pid is %d\n", result, getpid());
	return OK;
}