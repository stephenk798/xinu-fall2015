/* freemem.c - freemem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  freemem  -  Free a memory block, returning the block to the free list
 *------------------------------------------------------------------------
 */
syscall	freemem(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*next, *prev, *block;
	uint32	top;
	struct memblk *prevgbg;
	struct memblk *nextgbg;

	kprintf("freeingsize: %u, mem : 0x%08X\n", nbytes, blkaddr);
	mask = disable();
	if ((nbytes == 0) || ((uint32) blkaddr < (uint32) minheap)
			  || ((uint32) blkaddr > (uint32) maxheap)) {
		restore(mask);
		return SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	block = (struct memblk *)blkaddr;

	prev = &memlist;			/* Walk along free list	*/
	next = memlist.mnext;
	while ((next != NULL) && (next < block)) {
		prev = next;
		next = next->mnext;
	}

	if (prev == &memlist) {		/* Compute top of previous block*/
		top = (uint32) NULL;
	} else {
		top = (uint32) prev + prev->mlength;
	}

	/* Ensure new block does not overlap previous or next blocks	*/

	if (((prev != &memlist) && (uint32) block < top)
	    || ((next != NULL)	&& (uint32) block+nbytes>(uint32)next)) {
		restore(mask);
		return SYSERR;
	}

	memlist.mlength += nbytes;

	kprintf("Bout to get to gbglist freemem stuff\n");
	if(gbglist.mlength > 0){
		/* Remove allocated mem from list */
		prevgbg = &gbglist; //do the same as above walking along the freelist
		nextgbg = gbglist.gbgnext;
		
		//walk along until the block is found, or end of list is reached
		while( nextgbg != NULL && nextgbg != block){
			prevgbg = nextgbg; 
			nextgbg = nextgbg->gbgnext;
		}

		//found the block! time to get rid of its stuff, or end of list so don't free anything
		if(nextgbg != NULL){ 
			prevgbg->gbgnext = nextgbg->gbgnext;//remove entry from gbglist
			nextgbg = NULL;
			gbglist.mlength -= nbytes;
			block->gbgnext = NULL; //set the next to null
			block->gbgpid = -1; //Isn't owned by any process anymore, so set to -1
		}
	}

	kprintf("Made it past gbgfreemem stuff\n");
	/* Either coalesce with previous block or add to free list */

	if (top == (uint32) block) { 	/* Coalesce with previous block	*/
		prev->mlength += nbytes;
		block = prev;
	} else {			/* Link into list as new node	*/
		block->mnext = next;
		block->mlength = nbytes;
		prev->mnext = block;
	}

	/* Coalesce with next block if adjacent */

	if (((uint32) block + block->mlength) == (uint32) next) {
		block->mlength += next->mlength;
		block->mnext = next->mnext;
	}
	restore(mask);
	return OK;
}
