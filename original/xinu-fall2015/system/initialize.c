/* initialize.c - nulluser, sysinit, sizmem */

/* Handle system initialization and become the null process */

#include <xinu.h>
#include <string.h>

extern	void	start(void);	/* Start of Xinu code			*/
extern	void	*_end;		/* End of Xinu code			*/

/* Function prototypes */

extern	void main(void);	/* Main is the first process created	*/
extern	void xdone(void);	/* System "shutdown" procedure		*/
static	void sysinit(); 	/* Internal system initialization	*/
extern	void meminit(void);	/* Initializes the free memory list	*/

/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
struct	memblk	memlist;	/* List of free memory blocks		*/

/* Active system status */

int	prcount;		/* Total number of live processes	*/
pid32	currpid;		/* ID of currently executing process	*/
struct ts_disptb tsdtab[DISPTBSIZE];
qid16 mlfprocqueue[DISPTBSIZE];

void initTSTable(){
	int i;
	struct ts_disptb *level;
	for(i = 0 ; i < DISPTBSIZE; i++){
		level = &tsdtab[i];

		level->ts_tqexp = i-10;
		if(i < 10){
			level->ts_tqexp = 0;
			level->ts_slpret = 50;
			level->ts_quantum = 200;
		}
		else if(i < 20){
			level->ts_slpret = 51;
			level->ts_quantum = 160;
		}
		else if(i < 30){
			level->ts_slpret = 52;
			level->ts_quantum = 120;
		}
		else if(i < 35){
			level->ts_slpret = 53;
			level->ts_quantum = 80;
		}
		else if(i < 40){
			level->ts_slpret = 54;
			level->ts_quantum = 80;
		}
		else if(i < 50){
			if(i < 45){
				level->ts_slpret = 55;
			}
			else if (i == 45){
				level->ts_slpret = 56;
			}
			else if (i == 46){
				level->ts_slpret = 57;
			}
			else{
				level->ts_slpret = 58;
			}				
			level->ts_quantum = 40;
		}
		else if (i < 59){
			level->ts_slpret = 58;
			level->ts_quantum = 40;
		}
		else if (i == 59){
			level->ts_slpret = 59;
			level->ts_quantum = 20;
		}
	}
}

/*------------------------------------------------------------------------
 * nulluser - initialize the system and become the null process
 *
 * Note: execution begins here after the C run-time environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  The code turns itself into the null process
 * after initialization.  Because it must always remain ready to execute,
 * the null process cannot execute code that might cause it to be
 * suspended, wait for a semaphore, put to sleep, or exit.  In
 * particular, the code must not perform I/O except for polled versions
 * such as kprintf.
 *------------------------------------------------------------------------
 */

void	nulluser()
{	
	struct	memblk	*memptr;	/* Ptr to memory block		*/
	uint32	free_mem;		/* Total amount of free memory	*/
	
	/* Initialize the system */
		
	sysinit();

	kprintf("\n\r%s\n\n\r", VERSION);
	
	/* Output Xinu memory layout */
	free_mem = 0;
	for (memptr = memlist.mnext; memptr != NULL;
						memptr = memptr->mnext) {
		free_mem += memptr->mlength;
	}
	kprintf("%10d bytes of free memory.  Free list:\n", free_mem);
	for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
	    kprintf("           [0x%08X to 0x%08X]\r\n",
		(uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
	}

	kprintf("%10d bytes of Xinu code.\n",
		(uint32)&etext - (uint32)&text);
	kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)&text, (uint32)&etext - 1);
	kprintf("%10d bytes of data.\n",
		(uint32)&ebss - (uint32)&data);
	kprintf("           [0x%08X to 0x%08X]\n\n",
		(uint32)&data, (uint32)&ebss - 1);

	/* Enable interrupts */

	enable();

	/*Moved welcome message from main.c to here on 9/1/15*/
	kprintf("\nWelcome!\n");
	/*Dope name thing added 9/7/15*/
	kprintf("\n\n%s%s\n%s\n%s\n%s\n%s\n%s\n",WELCOME_BAN0, WELCOME_BAN1, WELCOME_BAN2, WELCOME_BAN3, WELCOME_BAN4, WELCOME_BAN5, WELCOME_BAN6, WELCOME_BAN7);
	kprintf("\n\033[0;97m I was modified by Stephen Kramer, kramer40!\n");
	kprintf("\nI am being printed from nulluser()in system/intialize.c.\n");
	kprintf("\nEventually I will turn into the do-nothing null process, and thats okay.\n");
	kprintf("\nI'm gonna cal the main function using create, which will be the first app\n");
	kprintf("\nThat app will create a second XINU app that runs shell in shell/shell.c.\n");
	/*
	kprintf("\nHello World!\n");
	kprintf("\nI'm the first XINU app and running function main() in system/main.c.\n");
	kprintf("\nI was created by nulluser() in system/initialize.c using create().\n");
	kprintf("\nMy creator will turn itself into the do-nothing null process.\n");
	kprintf("\nI will create a second XINU app that runs shell() in shell/shell.c as an example.\n");
	kprintf("\nYou can do something else, or do nothing; it's completely up to you.\n");
	*/
	

	/* Create a process to execute function main() */
	resume (
	   create((void *)main, INITSTK, INITPRIO, "Main process", 0,
           NULL));

	/* Become the Null process (i.e., guarantee that the CPU has	*/
	/*  something to run when no other process is ready to execute)	*/

	while (TRUE) {
		;		/* Do nothing */
	}

}

/*------------------------------------------------------------------------
 *
 * sysinit  -  Initialize all Xinu data structures and devices
 *
 *------------------------------------------------------------------------
 */
static	void	sysinit()
{
	int32	i;
	struct	procent	*prptr;		/* Ptr to process table entry	*/
	struct	sentry	*semptr;	/* Ptr to semaphore table entry	*/

	/* Platform Specific Initialization */

	platinit();

	/* Initialize the interrupt vectors */

	initevec();
	
	/* Initialize free memory list */
	
	meminit();

	/* Initialize system variables */

	/* Count the Null process as the first process in the system */

	prcount = 1;

	/* Scheduling is not currently blocked */

	Defer.ndefers = 0;

	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->prprio = 0;
	}

	/* Initialize the Null process entry */	

	prptr = &proctab[NULLPROC];
	prptr->prstate = PR_CURR;
	prptr->prprio = 0;
	strncpy(prptr->prname, "prnull", 7);
	prptr->prstkbase = getstk(NULLSTK);
	prptr->prstklen = NULLSTK;
	prptr->prstkptr = 0;
	currpid = NULLPROC;
	
	/* Initialize semaphores */

	for (i = 0; i < NSEM; i++) {
		semptr = &semtab[i];
		semptr->sstate = S_FREE;
		semptr->scount = 0;
		semptr->squeue = newqueue();
	}

	/* Initialize buffer pools */

	bufinit();

	/* initialize timeshare table*/
	initTSTable();

	/* Create a ready list for processes */

	for(i = 0; i < 60; i++){
		mlfprocqueue[i] = newqueue();
	}

	readylist = newqueue();
	

	/* Initialize the real time clock */

	clkinit();

	for (i = 0; i < NDEVS; i++) {
		init(i);
	}
	return;
}

int32	stop(char *s)
{
	kprintf("%s\n", s);
	kprintf("looping... press reset\n");
	while(1)
		/* Empty */;
}

int32	delay(int n)
{
	DELAY(n);
	return OK;
}
