#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800
#define EARTH_RADIUS 0.63781
#define MOON_MINOR (6.026857528*EARTH_RADIUS)
#define MOON_MAJOR (6.01776817*EARTH_RADIUS)
#define PI 3.1415926

void sphereDisplay(void);
void drawSphere(double);
void drawEarth();
void drawMoon();
void initLight();
void drawMoonOrbit();

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Sphere");

	glutDisplayFunc(sphereDisplay);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1, 0.1, 50);
	gluLookAt(7.0, 7.0, 7.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//initLight();

	glutMainLoop();
}

void sphereDisplay(void) {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glLineWidth(1.0);

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);  // X axis (red).
		glVertex3f(-15.0, 0.0, 0.0);
		glVertex3f(15.0, 0.0, 0.0);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);  // Y axis (green).
		glVertex3f(0.0, -15.0, 0.0);
		glVertex3f(0.0, 15.0, 0.0);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);  // Z axis (blue).
		glVertex3f(0.0, 0.0, -15.0);
		glVertex3f(0.0, 0.0, 15.0);
	glEnd();

	for(float i = -15; i <= 15; i++){
		glPointSize(2.0);
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POINTS);
			glVertex3f(i, 0.0, 0.0);
			glVertex3f(0.0, i, 0.0);
			glVertex3f(0.0, 0.0, i);
		glEnd();
	}

	glColor3f(0.5, 0.5, 0.5);
	drawEarth();

	glFlush();
	glutSwapBuffers();
}

void initLight(){

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHT0);
	GLfloat diffColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffColor);
	GLfloat pos[] = { 0.0, 15.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

}

void drawEarth() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(23.439281, 0.0, 0.0, 1.0);
	drawSphere(EARTH_RADIUS);
	drawMoon();
	glPopMatrix();
}

void drawMoon() {

	drawMoonOrbit();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(MOON_MAJOR, 0.0, 0.0);
	glRotatef(6.68, 0.0, 0.0, 1.0);
	drawSphere(EARTH_RADIUS*0.273);
	glPopMatrix();
}

void drawMoonOrbit(){

	glColor3f(0.0,0.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRotatef(5.14, 1.0, 0.0, 0.0);

	double x, z;

	glLineWidth(1.5);
	glBegin(GL_LINES);
    for(double i = 0; i <= 360; i += 0.01){

		x = MOON_MAJOR * cos((PI/180)*i);
		z = MOON_MINOR * sin((PI/180)*i);

		glVertex3d(x, 0.0, z);

    }
    glEnd();

    glPopMatrix();

}

void drawSphere(double radius) {
	GLUquadricObj* sph = gluNewQuadric();
	gluQuadricDrawStyle(sph, GLU_LINE);

	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluSphere(sph, radius, 20, 20);
	glPopMatrix();
}
