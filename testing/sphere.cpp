#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800
#define EARTH_RADIUS 0.63781

void sphereDisplay(void);
void drawSphere(double, double, double, double);
void drawEarth();
void drawMoon();

void mouseClick(int, int, int, int);
void moveCamera(int, int);

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Sphere");

	glutDisplayFunc(sphereDisplay);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1, 0.1, 50);
	gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHT0);
	GLfloat diffColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffColor);
	GLfloat pos[] = { 0.0, 15.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glutMainLoop();
}

void sphereDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glPointSize(3.0);
	for (float i = -15.0; i <= 15; i++) {
		glBegin(GL_POINTS);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(i, 0, 0); //x axis
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, i, 0); //y axis
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0, 0, i); //z axis
		glEnd();
	}

	glColor3f(0.5, 0.5, 0.5);
	drawEarth();

	glFlush();
	glutSwapBuffers();
}

void drawEarth() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(23.439281, 0.0, 0.0, 1.0);
	drawSphere(EARTH_RADIUS, 0.0, 0.0, 0.0);
	drawMoon();
	glPopMatrix();
}

void drawMoon() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(23.439281, 0.0, 0.0, 1.0);
	drawSphere(EARTH_RADIUS*0.273, 6.0, 0.0, 6.0);
	glPopMatrix();
}

void drawSphere(double radius, double cx, double cy, double cz) {
	GLUquadricObj* sph = gluNewQuadric();
	gluQuadricDrawStyle(sph, GLU_FILL);

	glPushMatrix();
	glTranslatef(cx, cy, cz);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluSphere(sph, radius, 100, 100);
	glPopMatrix();
}
