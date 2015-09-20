#include <xinu.h>
void printsegaddress(){
	kprintf("Text\n Value: 0x%08X \n End: 0x%08X\n Size: %10d\n", (uint32)*(&etext), (uint32)(&etext - 1), (uint32)(&etext - &text));
}
