#include <xinu.h>
#include <stdio.h>

#define INNERB 123456
#define OUTERB 5

void printloop(char c){
	int i = 0; 
	int j = 0;
	for( i < OUTERB; i++;){
		kprintf("%c", c);
		for(j < INNERBl j++){
		}
	}
	return;
}