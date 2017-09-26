#include <stdio.h>

void fib(int prevprev, int prev, int count, int max) {
	if(count < max) {
		printf("%d", prevprev);
		fib(prev,prev+prevprev, ++count, max);
	}
}

int main() {
	fib(1,1,0,5);
}