/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

process	main(void)
{
	/*Moved hello world message to intialize.c on 9/1/15*/

	/*test code for lab 1, host2netl and printsegaddress*/

	// stacktrace(currpid);

	// kprintf("-----Stack depth test-----\n");
	// stackdepth();

	// kprintf("-----DOne-----\n");

	// kprintf("0x12345678 turns into 0x%08X\n", host2netl(0x12345678));
	// resume(create(printsegaddress, 65536, 20, "printsegaddress", 0, NULL));
	// kprintf("\n");
	// sleepms(300);

	pid32 procA = create(printnoloop, 1024, 20, "printnoloop", 1, 'A');
	pid32 procB = create(printnoloop, 1024, 20, "printnoloop", 1, 'B');
	pid32 procC = create(printnoloop, 1024, 20, "printnoloop", 1, 'C');
	pid32 procD = create(printnoloop, 1024, 20, "printnoloop", 1, 'D');

	kprintf("P");
	resume(procA);
	sleepms(100);
	kprintf("P");
	resume(procB);
	sleepms(100);
	kprintf("P");
	resume(procC);
	sleepms(100);
	kprintf("P");
	resume(procD);
	sleepms(100);
	kprintf("Main dpeth: %d\n",stackdepth());
	sleepms(10000);
	kprintf("\n");
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