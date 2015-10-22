struct mlfqueue { qid16	queues[DISPTBSIZE];	/* different level queues*/
};

extern struct mlfqueue mlfprocqueue;
extern pid32 mlfqinsert(struct mlfqueue, pid32, int32);
extern pid32 mlfqdequeue(struct mlfqueue);
extern bool mlfqisempty(struct mlfqueue);