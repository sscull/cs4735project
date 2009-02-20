#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

#include "constants.h"
#include "universe.h"

#define WINDOW_WIDTH 1024.0
#define WINDOW_HEIGHT 768.0

using namespace solarSystem;
void init();
void initProjection();
void initLight();
void createUniverse();
void display();
void idle();
void drawBody(CelestialBody);
void printCenter(CelestialBody);

CelestialBody * sun;
double t;
GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	init();

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	createUniverse();

	glutMainLoop();
	return 0;
}

void init() {
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Universe");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	initProjection();
	initLight();
}

void initProjection() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(4.0, WINDOW_WIDTH/WINDOW_HEIGHT, 0.1, 1000 * AU);
	//gluLookAt(150.661699928, -29.99, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0); //earth
	gluLookAt(155.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initLight() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat pos[] = { 800.0, 0.0, 0.0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void createUniverse() {
	t = 0.0;
	sun = new Planet("sun", SUN_RADIUS, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
			0.0);
	CelestialBody * mercury = new Planet("mercury", MERCURY_RADIUS*50,
			MERCURY_MAJOR, MERCURY_E, MERCURY_APH, MERCURY_PER,
			MERCURY_OMEGA, MERCURY_P, 1.0, 1.0, 1.0);
	CelestialBody * venus = new Planet("venus", VENUS_RADIUS*50, VENUS_MAJOR,
	VENUS_E, VENUS_APH, VENUS_PER, VENUS_OMEGA, VENUS_P, 1.0, 1.0, 0.4);
	CelestialBody * earth = new Planet("earth", EARTH_RADIUS*50, EARTH_MAJOR,
	EARTH_E, EARTH_APH, EARTH_PER, EARTH_OMEGA, EARTH_P, 0.0, 0.0, 1.0);
	CelestialBody * moon = new Planet("moon", MOON_RADIUS, MOON_MAJOR, MOON_E,
	MOON_APH, MOON_PER, MOON_OMEGA, MOON_P, 0.7, 0.7, 0.7);
	earth->addChild(*moon);

	CelestialBody * mars = new Planet("mars", MARS_RADIUS, MARS_MAJOR, MARS_E,
	MARS_APH, MARS_PER, MARS_OMEGA, MARS_P, 1.0, 0.2, 0.2);
	CelestialBody * jupiter = new Planet("jupiter", JUPITER_RADIUS,
	JUPITER_MAJOR, JUPITER_E, JUPITER_APH, JUPITER_PER,
	JUPITER_OMEGA, JUPITER_P, 1.0, 0.5, 0.25);

	sun->addChild(*mercury);
	sun->addChild(*venus);
	sun->addChild(*earth);
	sun->addChild(*mars);
	sun->addChild(*jupiter);
	printCenter(*sun);
}

void printCenter(CelestialBody p) {
	std::cout << p.getName() << " " << p.getCenter().x << " "
			<< p.getCenter().y << " " << p.getCenter().z << std::endl;
	if (int c = p.hasChildren()) {
		for (int i = 0; i < c; i++) {
			printCenter(p.getChild(i));
		}
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	drawBody(*sun);
	glFlush();
	glutSwapBuffers();
}

void idle() {
	t += 1.0;
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void drawBody(CelestialBody p) {
	glPushMatrix();
	GLfloat color[] = { p.getRed(), p.getGreen(), p.getBlue() };
	glColor3f(color[0], color[1], color[2]);
	if (p.getName() == "sun") {
				glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
	} else {
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	}
	double pt = t / p.getPeriod();
	double x = p.getCenter().x + (p.getMajor() * cos(pt * M_PI / 180.0) * cos(
			p.getOmega() * M_PI / 180.0)) - (p.getMinor() * sin(pt * M_PI
			/ 180.0) * sin(p.getOmega() * M_PI / 180.0));
	double y = p.getCenter().y + (p.getMajor() * cos(pt * M_PI / 180.0) * sin(
			p.getOmega() * M_PI / 180.0)) + (p.getMinor() * sin(pt * M_PI
			/ 180.0) * cos(p.getOmega() * M_PI / 180.0));
	glTranslated(x, y, 0);
	glutSolidSphere(p.getRadius(), 10, 10);
	if (p.getName() == "earth") {
		std::cout << p.getName() << " x:" << x << " y:" << y << std::endl;
	} else if (p.getName() == "moon") {
		std::cout << p.getName() << " x:" << p.getCenter().x << " y:"
				<< p.getCenter().y << std::endl;
	}
	if (int c = p.hasChildren()) {
		for (int i = 0; i < c; i++) {
			Point* newCenter = new Point(x, y, 0.0);
			p.getChild(i).setCenter(*newCenter);
			drawBody(p.getChild(i));
		}
	}
	glPopMatrix();
}
