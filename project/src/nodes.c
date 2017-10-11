#include <stdlib.h>
#include <time.h>
#include "nodes.h"

struct Node{
	int x;
	int y;
	float velocity;
	int xdir;
	int ydir;
};


int flockSize = 10;
struct Node** flock;

void initFlock(int size) {
	flockSize = size;
	flock = malloc(sizeof(struct Node) * flockSize);
	time_t t;
	srand((unsigned) time(&t));
	for(int i = 0; i < flockSize; i++) {
		flock[i]->x = (rand() % 1000);
		flock[i]->y = (rand() % 1000);
		flock[i]->velocity = (((float)(rand() % 10)) / 10.0);
		flock[i]->xdir = (rand() % 10);
		flock[i]->ydir = (rand() % 10);
	}
}

void getXYat(int index, int* x, int* y) {
	x =  &flock[index]->x;
	y =  &flock[index]->y;
}


