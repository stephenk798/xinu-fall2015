#include <xinu.h>
void printsegaddress(){
	kprintf("Text\n Text: 0x%8x \n Beginning: 0x%8x\n End: 0x%8x\n Size: %d", text, &text, &etext - 1, &etext - &text);
	kprintf("Data: %8x\n Beginning: %8x\n End: %8x\n",&data, &etext, &edata - 1);
}