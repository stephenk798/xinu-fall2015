/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

int myvictimglobal = 0;

process	main(void)
{

	// pid32 victim = create(myvictim, 2048, 20, "myvictim", 0, NULL);
	// pid32 attacker = create(myattacker, 2048, 20, "myattacker", 0, NULL);
	// resume(victim);
	// resume(attacker);
	pid32 procA = create(printloop, 1024, 20, "procA", 1, 'A');
	pid32 procB = create(printloop, 1024, 20, "procB", 1, 'B');
	pid32 procC = create(printloop, 1024, 50, "procC", 1, 'C');
	pid32 procD = create(printloop, 1024, 50, "procD", 1, 'D');

	kprintf("P");
	resume(procA);
	kprintf("P");
	resume(procB);
	kprintf("P");
	resume(procC);
	kprintf("P");
	resume(procD);
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