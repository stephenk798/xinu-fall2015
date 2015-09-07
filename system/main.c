/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

process	main(void)
{
	/*Moved hello world message to intialize.c on 9/1/15*/


	/*-------*/
	kprintf("\n...creating a shell\n");
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	
	/*added create lab0app1.c and lab0app2.c 9/7/15*/
	resume(create(lab0app1, 65536, 20, "lab0 app1", 0, NULL));
	resume(create(lab0app2, 65536, 20, "lab0 app2", 0, NULL));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}
