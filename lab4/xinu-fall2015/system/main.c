/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>


void sendA(pid32 pid, umsg32 msg){
	send(pid, msg);
}

void sendbtA(pid32 pid){
		kprintf("Time: %d\n", clktimefine);
		sendbt(pid, 'a', 10);
		sendbt(pid, 'b', 3);
		sendbt(pid, 'c', 15);

}

void recA(){
	int i = 0;
	umsg32 msgrec;
	for(i =0; i<3; i++){
		msgrec = receive();
		kprintf("msgreceived: %c at %d\n", msgrec, clktimefine);
	}
}
process	main(void)
{

	pid32 rec1, snd1;
	rec1 = create(recA, 1024, 20, "rec1", 0);
	snd1 = create(sendbtA, 1024, 20, "snd1", 1, rec1);
	resume(rec1);
	resume(snd1);

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
