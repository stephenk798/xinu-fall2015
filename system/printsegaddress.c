#include <xinu.h>
void printsegaddress(){
	kprintf("Text\n Text: 0x%8x \n addTrest: %8x\n Beginning: %8x\n End: %8x\n", text, &text, etext, &etext - 1);
	kprintf("Data: %8x\n Beginning: %8x\n End: %8x\n",&data, &etext, &edata - 1);
}