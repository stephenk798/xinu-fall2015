#include <xinu.h>

pid32 mlfqinsert(pid32 pid, int32 key){
	enqueue(pid, mlfprocqueue[key]);
	return pid;
}

pid32 mlfqdequeue(){
	pid32 currpid;
	int i;
	for( i = DISPTBSIZE-1; i >= 0; i--){
		if(!isempty(mlfprocqueue[i])){
			currpid = dequeue(mlfprocqueue[i]);
			if (currpid == NULLPROC){
				if(!isempty(mlfprocqueue[i])){
					enqueue(currpid, mlfprocqueue[i]);
					currpid = dequeue(mlfprocqueue[i]);	
				}
			}
			break;
		}
	}
	return currpid;
}