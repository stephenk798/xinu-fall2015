#include <xinu.h>

struct mlfqueue mlfprocqueue;

pid32 mlfqinsert(
		pid32 pid,
		struct mlfqueue q,
		int32		key		/* Key for the inserted process	*/
	)
{
	enqueue(pid, q.queues[key]);
	return pid;
}

//if nothing is found in the loop by deque, thats REALLY BAD, so we default to returning NULLPROC to be safe.
pid32 mlfqdequeue(struct mlfqueue q)
{
	int i;
	for(i = DISPTBSIZE-1; i >=0; i--){
		if(!isempty(q.queues[i])){
			return dequeue(q.queues[i]);
		}
	}
	return NULLPROC;
}

bool8 mlfqisempty(struct mlfqueue q){
	int i;
	for(i = DISPTBSIZE-1; i >=0; i--){
		if(!isempty(q.queues[i])){
			return isempty(q.queues[i]);
		}
	}
	return isempty(q.queues[0]);
}