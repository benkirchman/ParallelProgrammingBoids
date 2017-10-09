#include <stdlib.h>
static const int FLOCK_SIZE = 10;

static struct {
	float** x[FLOCK_SIZE];
	float** y[FLOCK_SIZE];
	float** velocity[FLOCK_SIZE];
} flock; 

void setupFlock() {
	srand((unsigned) time(&t));
	for(int i = 0; i < FLOCK_SIZE; i++) {
		flock.x[i] = rand() % 50;
		flock.y[i] = rand() % 50;
		flock.velocity[i] = ((float)(rand() % 10)) / 10.0;
	}
}


