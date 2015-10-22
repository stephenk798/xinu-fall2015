#include <xinu.h>

qid16 mlfprocqueue[DISPTBSIZE];

pid32 mlfqinsert(
		pid32 pid,
		int32		key		/* Key for the inserted process	*/
	)
{
	struct procent *ptold;
	ptold = &proctab[pid];
	kprintf("proc name: %s\n", ptold->name);
	enqueue(pid, mlfprocqueue[key]);
	return pid;
}

//if nothing is found in the loop by deque, thats REALLY BAD, so we default to returning NULLPROC to be safe.
pid32 mlfqdequeue()
{
	int i;
	for(i = DISPTBSIZE-1; i >=0; i--){
		if(!isempty(mlfprocqueue[i])){
			return dequeue(mlfprocqueue[i]);
		}
	}
	return NULLPROC;
}

bool8 mlfqisempty(){
	int i;
	for(i = DISPTBSIZE-1; i >=0; i--){
		if(!isempty(mlfprocqueue[i])){
			return isempty(mlfprocqueue[i]);
		}
	}
	return isempty(mlfprocqueue[0]);
}