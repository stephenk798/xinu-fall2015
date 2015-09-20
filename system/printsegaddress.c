#include <xinu.h>
//Prints out Start and end bytes for the Text, data, and bss segements, along with their size and first value.
//Added 9/20/15 for Lab1
void printsegaddress(){
	kprintf("Text:\n Start: 0x%08x\n End: 0x%08X\n Value: 0x%08x\n Size: %10d bytes\n", (uint32)&text, (uint32)&etext, (uint32)text, (uint32)((&etext - &text)*4));
	kprintf("Data:\n Start: 0x%08x\n End: 0x%08X\n Value: 0x%08x\n Size: %10d bytes\n", (uint32)&data, (uint32)&edata, (uint32)data, (uint32)((&edata - &data)*4));
	kprintf("Bss:\n Start: 0x%08x\n End: 0x%08X\n Value: 0x%08x\n Size: %10d bytes\n", (uint32)&bss, (uint32)&ebss, (uint32)bss, (uint32)((&ebss - &bss)*4));
}
