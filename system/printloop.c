#include <xinu.h>
#include <stdio.h>

#define INNERB 123456
#define OUTERB 5

void printloop(char c){
	int i;
	int j;
	for(i = 0; i < OUTERB; i++){
		kprintf("%c", c);
		for(j = 0; j < INNERB; j++){
		}
	}
	return;
}