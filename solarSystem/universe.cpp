#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

#include "constants.h"
#include "universe.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

using namespace solarSystem;
void init();
void initProjection();
void initLight();
void createUniverse();
void display();
void idle();
void drawBody(CelestialBody);
void drawOrbit(CelestialBody);
void lookAt(CelestialBody, double);
Point getPos(CelestialBody);

CelestialBody * sun;
double t;

int main(int argc, char **argv) {
	createUniverse();
	glutInit(&argc, argv);
	init();

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	initProjection();
	initLight();

	glutMainLoop();
	return 0;
}

void init() {
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Universe");
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void initProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(4.0, 1.0, 0.1, 1000 * AU);
	gluLookAt(798, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initLight() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHT0);
	GLfloat diffColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffColor);
	GLfloat pos[] = { 800.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void createUniverse() {
	t = 0.0;
	sun = new Planet(SUN_RADIUS, 0, 0, "sun", 1.0, 1.0, 0.0);
	CelestialBody * mercury = new Planet(MERCURY_RADIUS*10, MERCURY_MAJOR,
	MERCURY_E, "mercury", 1.0, 1.0, 1.0);
	CelestialBody * venus = new Planet(VENUS_RADIUS, VENUS_MAJOR, VENUS_E,
	"venus", 1.0, 0.0, 0.0);
	CelestialBody * earth = new Planet(EARTH_RADIUS, EARTH_MAJOR, EARTH_E,
	"earth", 0.4, 0.4, 1.0);
	CelestialBody * moon = new Planet(MOON_RADIUS, MOON_MAJOR, MOON_E,
	"moon", 0.5, 0.5, 0.5);
	earth->addChild(*moon);
	CelestialBody * mars = new Planet(MARS_RADIUS, MARS_MAJOR, MARS_E,
	"mars", 1.0, 0.0, 0.0);
	CelestialBody * jupiter = new Planet(JUPITER_RADIUS, JUPITER_MAJOR,
			JUPITER_E, "jupiter", 1.0, 0.4, 0.4);
	sun->addChild(*mercury);
	sun->addChild(*venus);
	sun->addChild(*earth);
	sun->addChild(*mars);
	sun->addChild(*jupiter);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	drawBody(*sun);
	glFlush();
	glutSwapBuffers();
}

void idle() {
	lookAt(*sun, 798.0);
	t += 0.01;
	glutPostRedisplay();
}

void drawBody(CelestialBody p) {
	GLUquadricObj* sph = gluNewQuadric();
	gluQuadricDrawStyle(sph, GLU_FILL);
	glPushMatrix();
	//drawOrbit(p);
	glTranslated(p.getMajor(), 0, 0);
	glColor3f(p.getRed(), p.getGreen(), p.getBlue());
	gluSphere(sph, p.getRadius(), 100, 100);
	std::cout << p.getName() << " " << p.getRadius() << " " << p.getMajor()
			<< " " << p.hasChildren() << std::endl;
	if (int c = p.hasChildren()) {
		for (int i = 0; i < c; i++) {
			drawBody(p.getChild(i));
		}
	}
	glPopMatrix();
}
void drawOrbit(CelestialBody p) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	double x, y;
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (double i = 0; i <= 360; i += 0.001) {
		x = p.getMajor() * cos((M_PI/180) * i);
		y = p.getMinor() * sin((M_PI/180) * i);
		glVertex3d(x, y, 0);
	}
	glEnd();
	glPopMatrix();
}

void lookAt(CelestialBody p, double distance) {
	Point point = getPos(p);
	double x = point.x;
	double y = point.y;
	double z = point.z;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotated(t*M_PI/180, 0.0, 0.0, 1.0);
	glTranslated(distance, 0.0, 0.0);
	glPopMatrix();
}

Point getPos(CelestialBody p) {
	Point point;
	point.x = p.getMajor();
	point.y = 0;
	point.z = 0;
	return point;
}
