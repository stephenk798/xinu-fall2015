/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

umsg32 msgglob;

void sendA(pid32 pid, umsg32 msg){
	kprintf("Time: %d pid: %d, msg: %c\n", clktimefine, currpid, msg);
	sendbt(pid, msg, 0);
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

int myalrmhandler(void) {
	msgglob = receive();
	kprintf("\n\nclktimefine end: %d\n", clktimefine);
	kprintf("myalrmhandler ran in pid: %d, received: %c\n\n", currpid, msgglob);
	return(OK);
}
int myxcpuhandler(void) {
	msgglob = receive();
	kprintf("\n\ncpuused end: %d\n", proctab[currpid].prcpuused);
	kprintf("myxcpuhandler ran in pid: %d, received: %c\n", currpid, msgglob);
	return(OK);
}

int regcbsig(){
	int a;
    registercbsig(MYSIGRECV, &myrecvhandler, NULL);
	registercbsig(MYSIGALRM, &myalrmhandler, 500);
    registercbsig(MYSIGXCPU, &myxcpuhandler, 300); 
    kprintf("clktimefine start: %d\n", clktimefine);
   	kprintf("cpuused start: %d\n", proctab[currpid].prcpuused);
    while(TRUE){
      a+=1; //to represent this process is doing some stuff.
      if(a %100){
      	kprintf("clktimefine: %d, cpuused: %d, a is :%d\n",clktimefine,proctab[currpid].prcpuused, a);
      }
    }
    return OK;
}

process	main(void)
{
	msgglob = '0';
	kprintf("msgglob: %c\n", msgglob);
	pid32 rec = create(regcbsig, 1024, 20, "regcbsig", 0, NULL);
	pid32 sndA = create(sendA, 1024, 20, "sndA", 2, rec, 'A');
	pid32 sndB = create(sendA, 1024, 20, "sndB", 2, rec, 'B');
	pid32 sndC = create(sendA, 1024, 20, "sndC", 2, rec, 'C');
	resume(sndA);
	resume(sndB);
	resume(sndC);
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
