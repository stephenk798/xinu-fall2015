/* getmem.c - getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*getmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;
	struct  memblk *gbgptr;

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	kprintf("----------GETMEM CALLED from pid %d----------\n", currpid);
	prev = &memlist;
	curr = memlist.mnext;
	kprintf("gbglist.mlength in getmem: %u\n", gbglist.mlength);
	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;

			gbgptr = &gbglist; //get the allocated list
			gbglist.mlength += nbytes;
			curr->gbgpid = currpid; //Set the pid that owns the block
			curr->gbgnext = gbgptr->gbgnext;//set the next gbg mem blk in list for current 
			gbgptr->gbgnext = curr; //set the first gbg mem blk to current

			
			restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			
			gbgptr = &gbglist; //get the allocated list
			gbglist.mlength += nbytes;
			//curr->mlength = nbytes; //set the length of the block to actual size
			curr->gbgpid = currpid; //Set the pid that owns the block
			curr->gbgnext = gbgptr->gbgnext;//set the next gbg mem blk in list for current 
			gbgptr->gbgnext = curr; //set the first gbg mem blk to current
			
			restore(mask);
			return (char *)(curr);
		} else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}

	kprintf("---MAP OF GBGLIST---\n");
	gbgptr = &gbglist;
	while(gbgptr!= NULL){
		kprintf("gbg pid: %d, mlength: %u gbgaddress: 0x%08X, gbgnext: 0x%08X\n", gbgptr->gbgpid, gbgptr->mlength, gbgptr, gbgptr->gbgnext)
	}
	kprintf("---MAP OF GBGLIST DONE---\n");
	restore(mask);
	return (char *)SYSERR;
}
