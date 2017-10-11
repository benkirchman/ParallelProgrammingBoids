#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
struct Node{
	int x;
	int y;
	int xdir;
	int ydir;
};

float pixelToFloat(int pixel);
void update();
void move(int index);
void look(int index);
double getDist(int i1, int i2);
int flockSize = 2000;
int windowSize = 1000;
struct Node** flock;
int pos = .5;

void init()
{
	glClearColor (0.0, 0.0, 1.0, 0.0);
	glMatrixMode (GL_PROJECTION);
	glShadeModel (GL_SMOOTH);
}
 
void drawTriangle(struct Node * node)
{
	glBegin( GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);  
	glVertex2f(node->x/windowSize, node->y/windowSize);
	glVertex2f((node->x - node->xdir)/windowSize, (node->y)/windowSize);
	glVertex2f((node->x)/windowSize, (node->y - node->ydir)/windowSize);
   	glEnd(); 
}
int delay = 10;
void display() {
	// Clear Color and Depth Buffers
	//glutTimerFunc(delay, display, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	//update();
	glBegin(GL_TRIANGLES);
	for(int i = 0; i< flockSize; i++) {
		look(i);
	}
	for(int i = 0; i< flockSize; i++) {
		move(i);
		float x1 = pixelToFloat(flock[i]->x);
		float y1 = pixelToFloat(flock[i]->y);
		float x2 = pixelToFloat((flock[i]->x - flock[i]->xdir) - (flock[i]->ydir/2));
		float y2 = pixelToFloat((flock[i]->y - flock[i]->ydir) + (flock[i]->xdir/2));
		float x3 = pixelToFloat((flock[i]->x - flock[i]->xdir) + (flock[i]->ydir/2));
		float y3 = pixelToFloat((flock[i]->y - flock[i]->ydir) - (flock[i]->xdir/2));
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
		glVertex2f(x3,y3);
	}
	glEnd();
	glutSwapBuffers();
}

void getVisibleBoids(int subject, int* avgx, int* avgy, int* avgxdir, int* avgydir) {
	int count = 0;
	for(int current = 0; current < flockSize; current++) {
		if(getDist(current,subject) < 100.0 && current != subject) {
			count++;
			*avgx+=flock[current]->x;
			*avgy+=flock[current]->y;
			*avgxdir += flock[current]->xdir;
			*avgydir += flock[current]->ydir;
		}
	}
	if(count > 0) {
		*avgx/=count;
		*avgy/=count;
		*avgxdir/=count;
		*avgydir/=count;
	}
}
int getTooCloseBoids(int subject, int* tooCloseAvgX, int* tooCloseAvgY) {
	int tooClose = 0;
	for(int i = 0; i < flockSize; i++) {
		if(getDist(subject,i) < 20.0 && i != subject) {
			tooClose++;
			*tooCloseAvgX += flock[i]->x;
			*tooCloseAvgY += flock[i]->y;
		}
	}
	if(tooClose > 0) {
		*tooCloseAvgX /= tooClose;
		*tooCloseAvgY /= tooClose;
	}
	return tooClose;
}

double getDist(int i1, int i2) {
	int xDist = flock[i1]->x - flock[i2]->x;
	int yDist = flock[i1]->y - flock[i2]->y;
	return sqrt(xDist * xDist + yDist * yDist);
}

int getRandV() {
	int random =  (rand() %11) -5;
	if(random != 0) {
		return random;
	}
	return getRandV();
}

//Nature, I have a question: is it possible that flocks can form if each bird only pays attention to one other bird at a time?
void look(int i) {
	//set xdir and ydir based on nearby boids
	//go away from the center of mass of all too close boids OR
	//allign direction to all nearby boids that are not too close 
	//and move toward the center of all nearby boids;
	int avgx = 0;
	int avgy = 0;
	int avgxdir = 0;
	int avgydir = 0;	
	getVisibleBoids(i, &avgx, &avgy, &avgxdir, &avgydir);
	flock[i]->xdir = avgxdir;
	flock[i]->ydir = avgydir;

	int tooCloseAvgX = 0;
	int tooCloseAvgY = 0;
	int tooClose = getTooCloseBoids(i, &tooCloseAvgX, &tooCloseAvgY);
	int tooCloseXdir = 0;
	int tooCloseYdir = 0;
	if(tooClose > 0) {
		//steer away from the avg location
		int xdist = flock[i]->x - tooCloseAvgX;
		int ydist = flock[i]->y - tooCloseAvgY;
		int xavgdir = flock[i]->xdir;
		int yavgdir = flock[i]->ydir;
		if(abs(xdist) < abs(ydist)) {
			if(ydist/3 != 0) {
				yavgdir += 3 * (ydist/abs(ydist));
				xavgdir += xdist/abs(ydist/3);
			}
		} else if(abs(xdist) > abs(ydist)) {
			if(xdist/3 != 0) {
				xavgdir += 3 * (xdist/abs(xdist));
				yavgdir += ydist/abs(xdist/3);
			}
		}
		flock[i]->xdir = (xavgdir + flock[i]->xdir )/2;
		flock[i]->ydir = (yavgdir + flock[i]->ydir)/2;;
	} else { 	
		int xdist = avgx - flock[i]->x;
		int ydist = avgy - flock[i]->y;
		int xavgdir = flock[i]->xdir;
		int yavgdir = flock[i]->ydir;
		if(abs(xdist) < abs(ydist)) {
			if(ydist/5 != 0) {
				xavgdir = xdist/abs(ydist/5);
				yavgdir = 5 * (ydist/abs(ydist));
			}
		} else if(abs(xdist) > abs(ydist)) {
			if(xdist/5 != 0) {
				yavgdir = ydist/abs(xdist/5);
				xavgdir = 5 * (xdist/abs(xdist));
			}
		}
		flock[i]->xdir = (flock[i]->xdir + xavgdir)/2;
		flock[i]->ydir = (flock[i]->ydir + yavgdir)/2;
	}
	int y = flock[i]->y;
	int x = flock[i]->x;
	/*if(x < 500) {
		if(y < 500) {
			flock[i]->ydir +=1;
		} else {
			flock[i]->xdir +=1;	
		}	
	} else {
		if(y < 500) {
			flock[i]->xdir -=1;
		} else {
			flock[i]->ydir -=1;	
		}
	}*/
	if(x < 50) {
		flock[i]->xdir +=1;
	} else if (x > 950) {
	       flock[i]->xdir -=1;
	}
       	if (y < 50) {
 		flock[i]->ydir +=1;
	} else if (y > 950) {
		flock[i]->ydir-=1;
	}
	if(flock[i]->xdir == 0) {
		flock[i]->xdir = getRandV();
	}
	
	if(flock[i]->ydir == 0) {
		flock[i]->ydir = getRandV();
	}
	int xdir = flock[i]->xdir;
	if(xdir < 1 && xdir > -1) {
		flock[i]->xdir = 1  * (xdir/abs(xdir));
	}
	int ydir = flock[i]->ydir;
	if(ydir < 1 && ydir > -1) {
		flock[i]->ydir = 1 * (ydir/abs(ydir));
	}
	flock[i]->xdir = fmax(-5, fmin(5,flock[i]->xdir));
	flock[i]->ydir = fmax(-5, fmin(5,flock[i]->ydir));
}

void move(int i) {
	flock[i]->x += flock[i]->xdir;
	flock[i]->y += flock[i]->ydir;
	if(flock[i]->x > windowSize) {
		flock[i]->x = 0;
	}
	if(flock[i]->x < 0) {
		flock[i]->x = windowSize;
	}
	if(flock[i]->y > windowSize) {
		flock[i]->y = 0;
	}
	if(flock[i]->y < 0) {
		flock[i]->y = windowSize;
	}
	glutPostRedisplay();
}
int count = 0;
void update() {
	look(count);
 	move(count);
	count++;
	if(count >= flockSize) {
		count = 0;
	}
	glutPostRedisplay();
}


float pixelToFloat(int pixels) {
	float glPoint = (float)(pixels - (windowSize/2));
	glPoint = glPoint/(windowSize/2);
}

int initViews (int argc, char** argv)
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,1000);
	glutCreateWindow("F l o c k");

	// register callbacks
	glutDisplayFunc(display);
	//timedfunc display
	//glutTimerFunc(delay, display, 1);
	//idlefunc update
	//glutIdleFunc(update);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}

void initFlock() {
	flock = malloc(sizeof(struct Node) * flockSize);
	time_t t;
	srand((unsigned) time(&t));
	for(int i = 0; i < flockSize; i++) {
		flock[i] = malloc(sizeof(struct Node));
		flock[i]->x = (rand() % 1000);
		flock[i]->y = (rand() % 1000);
		flock[i]->xdir = getRandV();
		flock[i]->ydir = getRandV();
	}
}

int main(int argc, char** argv) {
	initFlock();
	initViews(argc, argv);
}


