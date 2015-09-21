#include <xinu.h>
#include <stdio.h>

#define INNERB 123456
#define OUTERB 5

void printloop(char c){
	for(int i = 0; i < OUTERB; i++;){
		kprintf("%c", c);
		for(int j = 0; j < INNERB; j++){
		}
	}
	return;
}