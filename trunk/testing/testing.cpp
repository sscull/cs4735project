#include "./universe.h"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

using namespace solarSystem;
void init();
void createUniverse();
void display();
void idle();
void drawBody(CelestialBody);

CelestialBody * test;
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
	glutCreateWindow("Test universe");
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, WINDOW_WIDTH/768.0, 0.1, 100);
	gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	GLfloat pos[] = { 0.0, 0.0, 0.0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

}

void createUniverse() {
	t = 0.0;
	test = new Planet("test", 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0);
	CelestialBody * child1 = new Planet("child1", 0.5, 5.0, 0.5, 1.0, 1.0,
			180.0, 1.0, 0.0, 1.0, 1.0);
	CelestialBody * child11 = new Planet("child11", 0.25, 1.0, 0.0, 1.0, 1.0,
			0.0, 0.5, 1.0, 0.0, 0.0);
	//child1->addChild(*child11);

	CelestialBody * child2 = new Planet("child2", 0.5, 10.0, 0.5, 1.0, 1.0,
			180.0, 2.0, 0.0, 1.0, 0.0);
	CelestialBody * child21 = new Planet("child21", 0.15, 1.0, 0.5, 1.0, 1.0,
			180.0, 0.5, 0.0, 1.0, 1.0);
	CelestialBody * child22 = new Planet("child22", 0.25, 2.0, 0.5, 1.0, 1.0,
			180.0, 5.0, 1.0, 0.0, 1.0);
	child2->addChild(*child21);
	child2->addChild(*child22);

	test->addChild(*child1);
	//test->addChild(*child2);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	drawBody(*test);
	glFlush();
	glutSwapBuffers();
}

void idle() {
	t += 0.5;
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void drawBody(CelestialBody p) {
	glPushMatrix();

	GLUquadricObj * sph = gluNewQuadric();
	gluQuadricNormals(sph, GLU_SMOOTH);
	gluQuadricTexture(sph, GL_TRUE);

	double pt = t / p.getPeriod();
	double x = p.getCenter().x + (p.getMajor() * cos(pt * M_PI / 180.0) * cos(
			p.getOmega() * M_PI / 180.0)) - (p.getMinor() * sin(pt * M_PI
			/ 180.0) * sin(p.getOmega() * M_PI / 180.0));
	double y = p.getCenter().y + (p.getMajor() * cos(pt * M_PI / 180.0) * sin(
			p.getOmega() * M_PI / 180.0)) + (p.getMinor() * sin(pt * M_PI
			/ 180.0) * cos(p.getOmega() * M_PI / 180.0));
	glTranslated(x, y, 0);

	glColor3f(p.getRed(), p.getGreen(), p.getBlue());
	GLfloat color[] = { p.getRed(), p.getGreen(), p.getBlue() };
	if (p.getName() == "test") {
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, black);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
	} else {
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
	}
	gluSphere(sph, p.getRadius(), 100, 100);
	if (int c = p.hasChildren()) {
		for (int i = 0; i < c; i++) {
			drawBody(p.getChild(i));
		}
	}
	glPopMatrix();
}
