#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

void sphereDisplay(void);
void drawSphere(double);

void mouseClick(int, int, int, int);
void moveCamera(int, int);

int main(int argc, char **argv) {
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(0,0);
  
  glutCreateWindow("Sphere");

  glutDisplayFunc(sphereDisplay);
	glutMouseFunc(mouseClick);
	glutMotionFunc(moveCamera);

	glClearColor(0.0,0.0,0.0,0.0);
	glColor3f(0.0,0.0,0.0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	gluPerspective(90.0, 1, 0.1, 15);
  gluLookAt(10.0,10.0,10.0,0.0,0.0,0.0,0.0,1.0,0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHT0);
	GLfloat diffColor[] = { 1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffColor);
	GLfloat pos[] = { 0.0, 15.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

  glutMainLoop();
}

void sphereDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPointSize(3.0);
	for (float i=-15.0; i<=15; i++) {
		glBegin(GL_POINTS);
			glColor3f(1.0,0.0,0.0);
			glVertex3f(i,0,0);				//x axis
			glColor3f(0.0,1.0,0.0);
			glVertex3f(0,i,0);				//y axis
			glColor3f(0.0,0.0,1.0);
			glVertex3f(0,0,i);				//z axis
		glEnd();
	}

	glColor3f(0.5,0.5,0.5);
	drawSphere(5);

	glFlush();
	glutSwapBuffers();
}

void drawSphere(double radius) {
	GLUquadricObj* sph = gluNewQuadric();
	gluQuadricDrawStyle(sph, GLU_LINE);

	glPushMatrix();
	glRotatef(90.0,1.0,0.0,0.0);
	gluSphere(sph, radius, 100, 100);
	glPopMatrix();
}

void mouseClick(int button, int state, int x, int y) {
	if (button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){

  }
}

void moveCamera(int x, int y) {

}
