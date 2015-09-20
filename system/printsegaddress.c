#include <xinu.h>
void printsegaddress(){
	kprintf("Text\n Value: 0x%8x \n Beginning: 0x%8x\n End: 0x%8x\n Size: %d\n", (uint32)text, (uint32)&text, (uint32)(&etext - 1), (uint32)(&etext - &text));
	kprintf("Data\n Value: 0x%8x \n Beginning: 0x%8x\n End: 0x%8x\n Size: %d\n", (uint32)data, (uint32)&data, (uint32)(&edata - 1), (uint32)(&edata - &data));
	kprintf("Bss\n Value: 0x%8x \n Beginning: 0x%8x\n End: 0x%8x\n Size: %d\n", (uint32)bss, (uint32)&bss, (uint32)(&ebss - 1), (uint32)(&ebss - &bss));

}