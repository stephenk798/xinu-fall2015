/* send.c - send */

#include <xinu.h>

qid16 sendlist;

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if ((prptr->prstate == PR_FREE) || prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

syscall	sendbt(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg,		/* Contents of message		*/
	  int32		maxwait		/* Max wait time for blocking*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/

	if(maxwait < 0){
		kprintf("maxwait less than 0, thats not allowed\n");
		return SYSERR;
	}


	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	prptr = &proctab[pid];
	if ((prptr->prstate == PR_FREE)) {
		restore(mask);
		return SYSERR;
	}
	
	struct procent *sendpr;
	sendpr = &proctab[currpid];
	    

	kprintf("pid %d, message sent: %c", pid, msg);

	//Check if it has message
	if (prptr->prhasmsg){
		//set sndmsg to the message passed, change process state, set sndflag to 1
		sendpr->prstate = PR_SEND;
	    sendpr->sndmsg = msg;
	    sendpr->sndflag = 1;
	    //if theres a time, set delay and insert to sleepq
	    if(maxwait>0)
		{
		    insertd(currpid, sleepq, maxwait);
		}
		//insert it into send list, priority doesn't matter cuz FIFO
		insert(currpid, sendlist, 0);

		resched();
	}
	//If does not have a message act like send
	//Not using an else, because when it returns its gotta check
	//May not even need if statement here
	if(prptr->prhasmsg == FALSE){ 
		sendpr->sndflag = 0;
		prptr->prmsg = msg;		/* Deliver message		*/
		prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

		/* If recipient waiting or in timed-wait make it ready */

		if (prptr->prstate == PR_RECV) {
			ready(pid);
		} else if (prptr->prstate == PR_RECTIM) {
			unsleep(pid);
			ready(pid);
		}
	}
	else{
		//if it does have a message, well then return an error.
		kprintf("It had a message and hadn't received yet\n");
		restore(mask);
		return SYSERR;	
	}

	restore(mask);		/* Restore interrupts */
	return OK;
}