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
	struct ts_disptb *tbl;
	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

<<<<<<< HEAD
	// If state is SLEEP it is IO, if is CURR then it is CPU
	if (ptold->prstate == PR_SLEEP){
		tbl = &tsdtab[ptold->prprio];
		ptold->prprio = tbl->ts_slpret;
	}
	else if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		
		if(currpid != NULLPROC){
			tbl = &tsdtab[ptold->prprio];
			ptold->prprio = tbl->ts_tqexp;
=======
	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			return;
>>>>>>> parent of d911d6a... lab3 part3 setup
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
<<<<<<< HEAD
		enqueue(currpid, mlfprocqueue[ptold->prprio]);
	}

	/* Force context switch to highest priority ready process */
	kprintf("Old proc: %s, priority: %d\n", ptold->prname, ptold->prprio);
	int mlfqlevel = DISPTBSIZE-1;
	while(mlfqlevel>=0){
		if(!(isempty(mlfprocqueue[mlfqlevel]))){	
			currpid = dequeue(mlfprocqueue[mlfqlevel]);
			if(currpid==NULLPROC){			//if highest priority process is only NULL process then
				//check there is no other process in the ready list
				if(!(isempty(mlfprocqueue[mlfqlevel]))){
					enqueue(currpid, mlfprocqueue[mlfqlevel]);	//put the process in the ready list as current and enqueue null in the end
					currpid = dequeue(mlfprocqueue[mlfqlevel]);
				}
			}
			kprintf("broke at level %d\n", mlfqlevel);
			break;
		}
		mlfqlevel--;
	}
	ptnew = &proctab[currpid];
	kprintf("new proc pid: %d name is: %s\n", currpid, ptnew->prname);
	//Check if NULL process and make sure there are no other processes to run
	if(currpid == NULLPROC && !mlfqisempty()){
		kprintf("reinsert null proc\n");
		enqueue(currpid, mlfprocqueue[ptnew->prprio]);
		currpid = mlfqdequeue();
		ptnew = &proctab[currpid];
	}
	tbl = &tsdtab[ptnew->prprio];

	ptnew->prstate = PR_CURR;
	preempt = tbl->ts_quantum;		/* Reset time slice for process	*/
=======
		insert(currpid, readylist, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
>>>>>>> parent of d911d6a... lab3 part3 setup
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
