#include <xinu.h>

//use a circular array to implement the sending queue
void insert_sendq(pid32 receiver, pid32 sender){
	struct procent *prptr;
	prptr = &proctab[receiver];
	
	//make sure there is enough space in queue to insert
	if((prptr->sendhead == 0 && prptr->sendtail == SENDQSIZE-1) || (prptr->sendhead == prptr->sendtail+1)){
		kprintf("Not enough space in send queue to insert\n");
		return;
	}

	//If empty, set value of head and tail to 0 index
	if(prptr->sendhead == -1 && prptr->sendtail == -1){
		prptr->sendhead = 0;
		prptr->sendtail = 0;
	}
	else if (prptr->sendtail == SENDQSIZE-1){
		//circle back around
		prptr->sendtail = 0;
	}
	else{
		//increase otherwise
		prptr->sendtail++;
	}
	prptr->sendqueue[prptr->sendtail] = sender;
}

pid32 dequeue_sendq(pid32 receiver){
	struct procent *prptr;
	prptr = &proctab[receiver];

	//Make sure its not empty
	if(prptr->sendhead == -1){
		kprintf("Errory empty send queue\n");
		return -1;
	}
	
	//if not empty, set the return value to the head
	pid32 pid = prptr->sendqueue[prptr->sendhead];

	//if the head and tail are the same, meaning only one element, reset it to an empty queue
	if(prptr->sendhead ==prptr->sendtail){
		prptr-sendhead = -1;
		prptr->sendtail = -1;
	}
	else if(prptr->sendhead == SENDQSIZE-1){
		//circle around if we gotta
		prptr->sendhead = 0;
	}
	else{
		//Otherwise increment
		prptr->sendhead++;
	}

	return pid;
}

bool8 isempty_sendq(pid32 receiver){
	struct procent *prptr;
	prptr = &proctab[receiver];

	if(prptr->sendhead == -1 && prptr->sendtail == -1){
		return TRUE;
	}

	return FALSE;
}