#include <xinu.h>
void printsegaddress(){
	kprintf("Data\n Beginning: %8x\n End: %8x\n", &edata, &edata - 1);
}