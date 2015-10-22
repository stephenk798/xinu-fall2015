struct mlfqueue { qid16	queues[DISPTBSIZE];	/* different level queues*/
};

extern struct mlfqueue mlfprocqueue;
extern pid32 mlfqinsert(pid32, struct mlfqueue*, int32);
extern pid32 mlfqdequeue(struct mlfqueue*);
extern bool8 mlfqisempty(struct mlfqueue*);