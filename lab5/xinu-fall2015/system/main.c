/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

umsg32 msgglob;

void sendA(pid32 pid, umsg32 msg){
	kprintf("Time: %d pid: %d, msg: %c\n", clktimefine, currpid, msg);
	send(pid, msg);
	kprintf("time returned: %d pid: %d\n", clktimefine, currpid);
}

void sendbt1(pid32 pid, char msg, int delay){
	kprintf("Time: %d pid: %d, msg: %c\n", clktimefine, currpid, msg);
	sendbt(pid, msg, delay);
	kprintf("time returned: %d pid: %d\n", clktimefine, currpid);
	kprintf("msgglob is :%c\n", msgglob);
}

void sendbtA(pid32 pid){
		kprintf("Time: %d\n", clktimefine);
		sendbt(pid, 'a', 10);
		kprintf("Time: %d\n", clktimefine);
		sendbt(pid, 'b', 3);
		kprintf("Time: %d\n", clktimefine);
		sendbt(pid, 'c', 15);

}

void recA(){
	int i = 0;
	umsg32 msgrec;
	sleepms(5);
	for(i =0; i<4; i++){
		msgrec = receive();
		kprintf("msgreceived: %c at %d\n", msgrec, clktimefine);
	}
}

int myrecvhandler(void) {
	msgglob = receive();
	kprintf("myrecvhandler ran in pid: %d, received: %c\n", currpid, msgglob);
	return(OK);
}

int regCB(){
	kprintf("in regCB at %d\n", clktimefine);
	if (registercb(&myrecvhandler) != OK) {
		kprintf("recv handler registration failed\n");
		return 1;
	}
	while(TRUE){
		kprintf("msgglob changed! is: %c\n", msgglob);
	}
	kprintf("returning from regCB time: %d\n", clktimefine);
	return OK;
}

process	main(void)
{
	msgglob = '0';
	kprintf("msgglob: %c\n", msgglob);
	pid32 rec = create(regCB, 1024, 20, "regCB", 0, NULL);
	pid32 sndA = create(sendA, 1024, 20, "sndA", 2, rec, 'a');

	resume(sndA);
	resume(rec);
	// pid32 rec = create(recA, 1024, 20, "rec", 0, NULL);
	// pid32 sndA = create(sendbt1, 1024, 20, "sndA", 3, rec, 'a', 20);
	// pid32 sndB = create(sendbt1, 1024, 20, "sndB", 3, rec, 'b', 4);
	// pid32 sndC = create(sendbt1, 1024, 20, "sndC", 3, rec, 'c', 25);
	// pid32 sndD = create(sendbt1, 1024, 20, "sndD", 3, rec, 'd', 30);
	kprintf("msgglob is :%c\n", msgglob);
	// resume(sndA);
	// resume(sndB);
	// resume(sndC);
	// resume(sndD);
	// resume(rec);
	while(TRUE){

	}


	sleepms(500);
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
