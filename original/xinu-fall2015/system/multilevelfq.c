#include <xinu.h>

mlfqueue mlfprocqueue[DISPTBSIZE];

pid32 mlfqinsert(
		mlfqueue q, 
		pid32 pid,
		int32		key		/* Key for the inserted process	*/
	)
{
	enqueue(pid, q.queues[key]);
	return pid;
}

//if nothing is found in the loop by deque, thats REALLY BAD, so we default to returning NULLPROC to be safe.
pid32 mlfqdequeue(mlfqueue q)
{
	int i;
	for(i = DISPTBSIZE-1; i >=0; i--){
		if(!isempty(q.queues[i]){
			return dequeue(q.queues[i]);
		}
	}
	return NULLPROC;
}

bool mlfqisempty(mlfqueue q){
	int i;
	for(i = DISPTBSIZE-1; i >=0; i--){
		if(!isempty(q.queues[i])){
			return false;
		}
	}
	return true;
}