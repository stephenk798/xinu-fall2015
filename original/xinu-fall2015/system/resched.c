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

	// If state is SLEEP it is IO, if is CURR then it is CPU
	if (ptold->prstate == PR_SLEEP){
		ptold->prprio = tsdtab[ptold->prprio].ts_slpret;
	}
	else if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		
		if(currpid != NULLPROC){
			ptold->prprio = tsdtab[ptold->prprio].ts_tqexp;
		}

		// if (ptold->prprio > firstkey(readylist)) {
		// 	return;
		// }

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		mlfqinsert(currpid, &mlfprocqueue, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */

	currpid = mlfqdequeue(&mlfprocqueue);
	ptnew = &proctab[currpid];
	//Check if NULL process and make sure there are no other processes to run
	if(currpid == NULLPROC && !mlfqisempty(&mlfprocqueue)){
		mlfqinsert(currpid, &mlfprocqueue, ptnew->prprio);
		currpid = mlfqdequeue(&mlfprocqueue);
		ptnew = &proctab[currpid];
	}
	ptnew->prstate = PR_CURR;
	preempt = tsdtab[ptnew->prprio].ts_quantum;		/* Reset time slice for process	*/
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
