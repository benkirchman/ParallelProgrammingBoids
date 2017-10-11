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


int flockSize = 100;
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
float angle = 0.0f;

void display() {
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera

//	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLES);
		glVertex2f(-1.0f* angle,-1.0f*angle);
		glVertex2f( 1.0f * angle, 0.0f);
		glVertex2f( 0.0f, 1.0f * angle);
	glEnd();

	angle+=0.1f;
	if(angle >= 1.0f) {
		angle = 0.1f;
	}

	glutSwapBuffers();
}
  
int initViews (int argc, char** argv)
{
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,1000);
	glutCreateWindow("Lighthouse3D- GLUT Tutorial");

	// register callbacks
	glutDisplayFunc(display);
	glutIdleFunc(display);

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
		flock[i]->xdir = (rand() % 10);
		flock[i]->ydir = (rand() % 10);
	}
}

int main(int argc, char** argv) {
	initFlock();
	initViews(argc, argv);
}



