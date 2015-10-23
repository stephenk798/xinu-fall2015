/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

process	main(void)
{
	// pid32 victim = create(myvictim, 2048, 20, "myvictim", 0, NULL);
	// pid32 attacker = create(myattacker, 2048, 20, "myattacker", 0, NULL);
	// resume(victim);
	// resume(attacker);
	pid32 procA = create(iointensive, 1024, 1, "procA", 0, NULL);
	pid32 procB = create(iointensive, 1024, 1, "procB", 0, NULL);
	pid32 procC = create(cpuintensive, 1024, 1, "procC", 0, NULL);
	pid32 procD = create(cpuintensive, 1024, 1, "procD", 0, NULL);

	
	resume(procA);
	
	resume(procB);
	
	resume(procC);
	
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