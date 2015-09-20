#include <xinu.h>
void printsegaddress(){
	kprintf("Text:\n Start: 0x%08x\n End: 0x%08X\n Value: 0x%08x\n Size: %10d\n", (uint32)&text, (uint32)&etext, (uint32)etext, (uint32)(&etext - &text));
	kprintf("\nText: 0x%08X\n Etext: 0x%08X\n &text: 0x%08X\n &etext: 0x%08X\n &etext-1: 0x%08X\n *(etext-1): 0x%08X\n *(&etext): 0x%08X\n", (uint32)text, (uint32)etext, (uint32)&text, (uint32)&etext, (uint32)(&etext-1),(uint32)*(&etext-1), (uint32)*(&etext));
	kprintf("data: 0x%08X\n Edata: 0x%08X\n &data: 0x%08X\n &edata: 0x%08X\n &edata-1: 0x%08X\n *(edata-1): 0x%08X\n *(&edata): 0x%08X\n", (uint32)data, (uint32)edata, (uint32)&data, (uint32)&edata, (uint32)(&edata-1),(uint32)*(&edata-1), (uint32)*(&edata));

	// kprintf("Text\n Value: 0x%08X \n End: 0x%08X\n Size: %10d\n", (uint32)*(&etext), (uint32)(&etext - 1), (uint32)(&etext - &text));
}
