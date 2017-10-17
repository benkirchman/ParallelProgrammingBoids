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
void addToAvg(long i);
void printAvgToFile();
double getDist(int i1, int i2);
int flockSize = 2000;
int windowSize = 2000;
struct Node** flock;
int pos = .5;
FILE *fptr;

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
struct timespec start_time;
struct timespec end_time;
long msec;
void display() {
	// Start timer.
	clock_gettime(CLOCK_MONOTONIC, &start_time);
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
		float x2 = pixelToFloat((flock[i]->x - flock[i]->xdir) - (flock[i]->ydir/2)*2);
		float y2 = pixelToFloat((flock[i]->y - flock[i]->ydir) + (flock[i]->xdir/2)*2);
		float x3 = pixelToFloat((flock[i]->x - flock[i]->xdir) + (flock[i]->ydir/2)*2);
		float y3 = pixelToFloat((flock[i]->y - flock[i]->ydir) - (flock[i]->xdir/2)*2);
		float color1 = ((float)((flock[i]->xdir + 10)*10)+400)/600;
		float color2 = ((float)((flock[i]->ydir + 10)*10)+400)/600;
		float color3 = 200;//((float)((flock[i]->xdir + flock[i]->ydir + 20)*5)+400)/600;
		glColor3f(color1,color2,color3);
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
		glVertex2f(x3,y3);
	}
	glEnd();
	glutSwapBuffers();
	// End timer.
	clock_gettime(CLOCK_MONOTONIC, &end_time);
	msec = (end_time.tv_sec - start_time.tv_sec)*1000 + (end_time.tv_nsec - start_time.tv_nsec)/1000000;
	addToAvg(msec);
}

void getVisibleBoids(int subject, int* avgx, int* avgy, int* avgxdir, int* avgydir) {
	int count = 0;
	for(int current = 0; current < flockSize; current++) {
		if(getDist(current,subject) < 40.0 && current != subject) {
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
		if(getDist(subject,i) < 30.0 && i != subject) {
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
	int random =  (rand() %11);
	if(random != 5) {
		return random-5;
	}
	return getRandV();
}

//Nature, I have a question: is it possible that flocks can form if each bird only pays attention to one other bird at a time?
//The answer is no. That's rediculous, silly. 
void look(int i) {
	//set xdir and ydir based on nearby boids
	//go away from the center of mass of all too close boids OR
	//allign direction to all nearby boids that are not too close 
	//and move toward the center of all nearby boids;
	
	//xdir = xdir + xdirtoward + xdiraway + xavg + xbounddir
	int xdirTo = 0;
	int xdirAway = 0;
	int xdirAvg = 0;
	int xbounddir = 0;
	int ydirTo = 0;
	int ydirAway = 0;
	int ydirAvg = 0;
	int ybounddir = 0;
	
	int avgx = 0;
	int avgy = 0;
	getVisibleBoids(i, &avgx, &avgy, &xdirAvg, &ydirAvg);
	int xdist = avgx - flock[i]->x;	
	int ydist = avgy - flock[i]->y;
	if(xdist !=0) {
		xdirTo = xdist/abs(xdist);
	}
	if(ydist !=0) {
		ydirTo = ydist/abs(ydist);	
	}

	avgx = 0;
	avgy = 0;
	int tooClose = getTooCloseBoids(i, &avgx, &avgy);
	if(tooClose > 0) {
		xdist = flock[i]->x - avgx;
		ydist = flock[i]->y - avgy;
		if(xdist !=0) {
			xdirAway = 2 * xdist/abs(xdist);
			xdirTo = 0;
		}
		if(ydist != 0) {
			ydirAway = 2 * ydist/abs(ydist);
			ydirTo = 0;
		}
	}
		
	// Edge detection
	int ea = 1;
	int x = flock[i]->x;
	int y = flock[i]->y;	
	if(x < 100) {
		xbounddir = ea;
		//ybounddir = ea;
	}
	if (x > windowSize-100) {
	       xbounddir = -ea;
	       //ybounddir = -ea;
	}
       	if (y < 100) {
		//xbounddir = -ea;
		ybounddir = ea;
	}
	if (y > windowSize-100) {
		//xbounddir = ea;
		ybounddir = -ea;
	}

	//accumulate
	flock[i]->xdir =  xdirTo + xdirAway + xbounddir;
	flock[i]->ydir =  ydirTo + ydirAway + ybounddir;
	
	//cant stop wont stop
	if(sqrt(flock[i]->xdir * flock[i]->xdir + flock[i]->ydir * flock[i]->ydir) < 6) {
		flock[i]->xdir *=2;
		flock[i]->ydir *=2;
	}
	
	int min = -10;
	int max = 10;
	flock[i]->xdir = fmax(min, fmin(max,flock[i]->xdir));
	flock[i]->ydir = fmax(min, fmin(max,flock[i]->ydir));
	max+=1;
	//flip at edge
	if(x < max) {
		flock[i]->xdir = abs(flock[i]->xdir) + 1;
	} 
       	if (x > windowSize - max) {
		flock[i]->xdir = (-1 * abs(flock[i]->xdir)) - 1;
	}
       	if (y < max) {
		flock[i]->ydir = abs(flock[i]->ydir) + 1;
	} 
	if (y > windowSize - max) {
		flock[i]->ydir = (-1 * abs(flock[i]->ydir)) - 1;
	}
	
}	

void move(int i) {
	flock[i]->x += flock[i]->xdir;
	flock[i]->y += flock[i]->ydir;
	int min = 0;
	int max = windowSize;
	flock[i]->x = fmax(min, fmin(max,flock[i]->x));
	flock[i]->y = fmax(min, fmin(max,flock[i]->y));
	/*if(flock[i]->x > windowSize) {
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
	}*/
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

void initFlock(int size) {
	flockSize = size;
	flock = malloc(sizeof(struct Node) * flockSize);
	time_t t;
	srand((unsigned) time(&t));
	for(int i = 0; i < flockSize; i++) {
		flock[i] = malloc(sizeof(struct Node));
		flock[i]->x = (rand() % windowSize);
		flock[i]->y = (rand() % windowSize);
		flock[i]->xdir = getRandV();
		flock[i]->ydir = getRandV();
	}
}
long avg[100];
int counter=0;

void addToAvg(long i) {
	if(counter == 99) {
		printAvgToFile();
		counter = 0;
	} else {
		avg[counter] = i;
		counter++;
	}
}

void printAvgToFile() {
	fptr = fopen("/home/b/Documents/parallel/project/src/serial_avg.csv","a");
	long averageToPrint = 0;
	for(int i = 0; i < 99; i++) {
		averageToPrint += avg[i];
	}
	averageToPrint/=99;
	fprintf(fptr,"%d,%d\n", flockSize, averageToPrint);
	fclose(fptr);
}

void printFileHeader() {
	fptr = fopen("/home/b/Documents/parallel/project/src/serial_avg.txt","a");
	fprintf(fptr,"f#,mspf,serial\n");
	fclose(fptr);
}
int main(int argc, char** argv) {
	printFileHeader();
	initFlock(2000);
	initViews(argc, argv);
}



