#include <xinu.h>
#include <stdio.h>

#define INNERB 1000000
#define OUTERB 5

void printloop(char c){
	int i = 0; 
	int j = 0;
	while( i < OUTERB){
		kprintf("%c\n", c);
		while(j < INNERB){
			j++;
		}
		i++;
	}
	return;
}