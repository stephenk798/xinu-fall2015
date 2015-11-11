#include <xinu.h>

syscall registercb( int (* func) (void) ){
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/

	mask = disable();
	/*	if the cb func is NULL return an error	*/
	if(func == NULL){
		restore(mask);
		return SYSERR;
	}
	prptr = &proctab[currpid];
	prptr->cbfunc = func; //set the cbfunc of the current process to the callback function

	restore(mask);
	return OK;
}