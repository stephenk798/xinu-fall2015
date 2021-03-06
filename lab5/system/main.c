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
	msgglob++;
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
	msgglob++;
	kprintf("\n\nclktimefine end: %d\n", clktimefine);
	kprintf("myalrmhandler ran in pid: %d, changed msgglob to: %c\n\n", currpid, msgglob);
	return(OK);
}
int myxcpuhandler(void) {
	msgglob++;
	kprintf("\n\ncpuused end: %d\n", proctab[currpid].prcpuused);
	kprintf("myxcpuhandler ran in pid: %d, changed msgglob to: %c\n\n", currpid, msgglob);
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
    }
    return OK;
}

void sendmem(pid32 pid){
	kprintf("memlist mlength in bsnd: %u\n", memlist.mlength);
	char* memtest = getmem(30);
	char* memtest2 = getmem(70);
	freemem(memtest, 30);
	send(pid, 'A');
	
	kprintf("memlist mlength in snd: %u\n", memlist.mlength);
	return;
}

void receivemem(){
	kprintf("memlist in brec is: %u\n", memlist.mlength);
	char*memtest3 = getmem(40);
	msgglob = receive();
	msgglob++;
	kprintf("msgglob is: %c\n", msgglob);
	kprintf("memlist in erec is: %u\n", memlist.mlength);
	return;
}
process	main(void)
{
	kprintf("memlist in bmain is: %u\n", memlist.mlength);
	kprintf("gbglist in bmain is: %u\n", gbglist.mlength);
	kprintf("null looks like: 0x%08X\n", NULL);
	msgglob = '0';
	kprintf("msgglob: %c\n", msgglob);
	
	pid32 recmem = create(receivemem,1024,21,"recmem", 0, NULL);
	pid32 sndmem = create(sendmem,1024,21,"sndmem", 1, recmem);
	
	resume(recmem);
	resume(sndmem);
	//pid32 rec = create(regcbsig, 1024, 20, "regcbsig", 0, NULL);
	//pid32 sndA = create(sendA, 1024, 20, "sndA", 2, rec, 'A');
	// resume(sndA);
	// resume(rec);
	// pid32 rec = create(recA, 1024, 20, "rec", 0, NULL);
	// pid32 sndA = create(sendbt1, 1024, 20, "sndA", 3, rec, 'a', 20);
	// pid32 sndB = create(sendbt1, 1024, 20, "sndB", 3, rec, 'b', 4);
	// pid32 sndC = create(sendbt1, 1024, 20, "sndC", 3, rec, 'c', 25);
	// pid32 sndD = create(sendbt1, 1024, 20, "sndD", 3, rec, 'd', 30);
	
	//kprintf("msgglob is :%c\n", msgglob);
	// resume(sndA);
	// resume(sndB);
	// resume(sndC);
	// resume(sndD);
	// resume(rec);
	while(TRUE){
		kprintf("In main msgglob: %c\n", msgglob);
		sleepms(100);
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
