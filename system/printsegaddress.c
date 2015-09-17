#include <xinu.h>
void printsegaddress(){
	kprintf("Text\n Beginning: %8x\n End: %8x\n", &text, &etext - 1);
	kprintf("Data: %8x\n Beginning: %8x\n End: %8x\n",&data, &etext, &edata - 1);
}