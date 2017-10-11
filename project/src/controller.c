#include "view.h"
#include "nodes.h"

int main(int argc, char** argv) {
	int windowSize = 1000;
	initViews(argc, argv, windowSize);
	int flockSize = 10;
	initFlock(flockSize);
	clearView();
	for(int i = 0; i < flockSize; i++) {
		int x1;
		int y1;
		getXYat(i,&x1,&y1);
		float x[] = {x1/windowSize, (x1-5)/windowSize, (x1+5)/windowSize};
		float y[] = {y1/windowSize, (y1-10)/windowSize, (y1-10)/windowSize};
		drawTriangle(x,y);
	}
	flush();
}

