/* kill.c - kill */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	int32	i;			/* Index into descriptors	*/
	struct memblk *prevgbg;
	struct memblk *nextgbg;

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		restore(mask);
		return SYSERR;
	}

	kprintf("Killing process %s, pid: %d\n", prptr->prname, pid);
	kprintf("memlist mlength b4 free: %u\n", memlist.mlength);
	prevgbg = &gbglist;//Start the walking
	nextgbg = gbglist.gbgnext;
	while(nextgbg != NULL && nextgbg->gbgpid > 0 && gbglist.mlength > 0){
		//check the pid that owns curr mem, if they match then free it
		if(nextgbg->gbgpid == pid){
			kprintf("nextgbg mlength: %u\n",nextgbg->mlength);
			if(freemem((char *)nextgbg, nextgbg->mlength) != OK){ //Make sure it frees the memory aokay
				kprintf("mem did not free aokay...\n");
				restore(mask);
				return SYSERR;
			}
			nextgbg = prevgbg->gbgnext; //Continue walking
		
		}
		else{
			//if they don't match, set prev to next, set next to next 
			prevgbg = nextgbg;
			nextgbg = nextgbg->gbgnext;
		}
		//prevgbg->gbgnext is updated when freemem is called, otherwise it is updated in the else above
		kprintf("still in gbg walk, nextgbg pid: %d\n", nextgbg->gbgpid); //Stuck in this loop for some reason
	}
	kprintf("finsihed %s gbg list. memlist mlength now: %u\n", prptr->prname, memlist.mlength);
	if (--prcount <= 1) {		/* Last user process completes	*/
		xdone();
	}

	send(prptr->prparent, pid);
	for (i=0; i<3; i++) {
		close(prptr->prdesc[i]);
	}
	freestk(prptr->prstkbase, prptr->prstklen);

	switch (prptr->prstate) {
	case PR_CURR:
		prptr->prstate = PR_FREE;	/* Suicide */
		resched();

	case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		/* Fall through */

	case PR_READY:
		getitem(pid);		/* Remove from queue */
		/* Fall through */

	default:
		prptr->prstate = PR_FREE;
	}

	restore(mask);
	return OK;
}
