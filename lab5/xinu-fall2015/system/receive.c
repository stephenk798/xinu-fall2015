/* receive.c - receive */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/

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
