#include <xinu.h>

syscall registercbsig(uint16 asig, int ( *func)(void), uint32 optarg){
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/

	mask = disable();

	kprintf("Made it into registercb sig, asig: %d\n", asig);
	/*	if the cb func is NULL return an error	*/
	if(func == NULL){
		restore(mask);
		return SYSERR;
	}
	prptr = &proctab[currpid];
	
	if(asig == MYSIGRECV){
		kprintf("Successfully set cbfunc for asig\n");
		prptr->cbfunc = func; //set the cbfunc of the current process to the callback function
	}
	else if (asig == MYSIGALRM){
		prptr->alarmfunc = func; //Set alarm func 
		prptr->alarmtime = optarg + clktimefine; //Set wait time with optarg, then add curr time to make checking later easier
	}
	else if (asig == MYSIGXCPU){
		prptr->xcputime = optarg - (prptr->prcpuused) - (clktimefine - clktimeswitch);
	}
	restore(mask);
	return OK;
}