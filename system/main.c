/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

process	main(void)
{
	/*Moved hello world message to intialize.c on 9/1/15*/


	/*added create lab0app1.c and lab0app2.c 9/7/15*/
	kprintf("0x12345678 turns into 0x%08X\n", host2netl((long)0x12345678));
	resume(create(printsegaddress, 65536, 20, "printsegaddress", 0, NULL));
	sleepms(200);
	/*-------*/
	kprintf("\n...creating a shell\n");
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}
0x12345678
0000 0000 0001 0010 0011 0100 0101 0110 0111 1000
0x78563412
0000 0000 0111 1000 0101 0110 0011 0100 0001 0010