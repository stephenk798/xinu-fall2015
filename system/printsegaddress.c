#include <xinu.h>
void printsegaddress(){
	kprintf("Text: 0x%08X\n Etext: 0x%08X\n &text: 0x%08X\n &etext: 0x%08X\n &etext-1: 0x%08X\n *(etext-1): 0x%08X\n *(&etext): 0x%08X\n", (uint32)text, (uint32)etext, (uint32)&text, (uint32)&etext, (uint32)(&etext-1),(uint32)*(&etext-1), (uint32)*(&etext));
	// kprintf("Text\n Value: 0x%08X \n End: 0x%08X\n Size: %10d\n", (uint32)*(&etext), (uint32)(&etext - 1), (uint32)(&etext - &text));
}
