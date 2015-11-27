/* recvtime.c - recvtime */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  recvtime  -  Wait specified time to receive a message and return
 *------------------------------------------------------------------------
 */
umsg32	recvtime(
	  int32		maxwait		/* Ticks to wait before timeout */
        )
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent	*prptr;		/* Tbl entry of current process	*/
	umsg32	msg;			/* Message to return		*/

	if (maxwait < 0) {
		return SYSERR;
	}
	mask = disable();

	/* Schedule wakeup and place process in timed-receive state */

	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {	/* If message waiting, no delay	*/
		if (insertd(currpid,sleepq,maxwait) == SYSERR) {
			restore(mask);
			return SYSERR;
		}
		prptr->prstate = PR_RECTIM;
		resched();
	}

	/* Either message arrived or timer expired */

	if (prptr->prhasmsg) {
		msg = prptr->prmsg;	/* Retrieve message		*/
		prptr->prhasmsg = FALSE;/* Reset message indicator	*/
	} else {
		msg = TIMEOUT;
	}

	pid32 sendid;
	struct procent *sendptr;
	//while there are itesm in send list, go through them until
	//a sending process with a valid msg is found, then break	
	while(!isempty_sendq(currpid)){
		sendid = dequeue_sendq(currpid);
		sendptr = &proctab[sendid];
		if(sendptr->sndflag == 1){
			break;
		}
	}
	
	//Remove the process from the sleep queue, and if its good ready it
	if(unsleep(sendid) == OK){
		ready(sendid);
	}
	
	restore(mask);
	return msg;
}
