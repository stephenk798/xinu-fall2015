#include <xinu.h>
#include <stdio.h>

#define INNERB 123456
#define OUTERB 5

//I dont think this needs comments, but good to be safe
void printloop(char c){
	int i;
	int j; //declare variables
	for(i = 0; i < OUTERB; i++){
		kprintf("%c", c); //print char
		for(j = 0; j < INNERB; j++){
			//do nothing for a while
		}
	}
	return;
}

void printnoloop(char c){
	kprintf("%c", c);
	kprintf("depth: %d", stackdepth());
	sleepms(10000);
}