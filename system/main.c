/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

int myvictimglobal = 0;

process	main(void)
{

	pid32 victim = create(myvictim, 2048, 20, "myvictim", 0, NULL);
	pid32 attacker = create(myattacker, 2048, 20, "myattacker", 0, NULL);
	kprintf("vicpid: %d attackpid: %d\n", victim, attacker);
	kprintf("---Vicim and Attack---\n");
	resume(victim);
	resume(attacker);
	sleepms(3000);
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