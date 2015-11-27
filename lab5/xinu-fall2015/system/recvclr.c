/* recvclr.c - recvclr */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  recvclr  -  Clear incoming message, and return message if one waiting
 *------------------------------------------------------------------------
 */
umsg32	recvclr(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == TRUE) {
		msg = prptr->prmsg;	/* Retrieve message		*/
		prptr->prhasmsg = FALSE;/* Reset message flag		*/
	} else {
		msg = OK;
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
