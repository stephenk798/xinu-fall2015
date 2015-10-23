/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];
	ptold->prcpuused += (clktimefine - clktimeswitch);
	clktimeswitch = clktimefine;/*reset time switched in */	
	// if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
	// 	if (ptold->prprio > firstkey(readylist)) {
	// 		return;
	// 	}

	// 	/* Old process will no longer remain current */

	// 	ptold->prstate = PR_READY;
	// 	insert(currpid, readylist, ptold->prprio);
	// }

	struct ts_disptb *level = &tsdtab[ptold->prprio];
	if(ptold->prstate == PR_SLEEP){
		//if process is asleep it is IO
		ptold->prprio = level->ts_slpret;
	}
	else if(ptold->prstate == PR_CURR){
		//if it is current it is CPU
		if(currpid != NULLPROC){
			ptold->prprio = level->ts_tqexp;
		}

		ptold->prstate = PR_READY;
		enqueue(currpid, mlfprocqueue[ptold->prprio]);
	}

	/* Force context switch to highest priority ready process */
	int i = DISPTBSIZE -1;
	while(i >= 0){
		if(!isempty(mlfprocqueue[i])){
			currpid = dequeue(mlfprocqueue[i]);
			if (currpid == NULLPROC){
				if(!isempty(mlfprocqueue[i])){
					enqueue(currpid, mlfprocqueue[i]);
					currpid = dequeue(mlfprocqueue[i]);	
				}
			}
			break;
		}
		i--;
	}
	ptnew = &proctab[currpid];
	level = &tsdtab[ptnew->prprio];
	ptnew->prstate = PR_CURR;
	preempt = level->ts_quantum;		/* Reset time slice for process	*/
	if(currpid != NULLPROC){
		kprintf("\nproc: %s New preempt: %d\n", ptnew->prname, preempt);
	}
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
