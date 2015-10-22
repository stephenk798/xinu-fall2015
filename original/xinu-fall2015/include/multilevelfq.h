struct mlfqueue { qid16	queues[DISPTBSIZE];	/* different level queues*/
};

extern mlfqueue mlfprocqueue;
extern pid32 mlfqinsert(mlfqueue, pid32, int32);
extern pid32 mlfqdequeue(mlfqueue);
extern bool mlfqisempty(mlfqueue);