#include "constants.h"
#include "universe.h"

using namespace solarSystem;
void mainLoop();
void init();
void initTextures();
void initDisplay(int&, char**);
bool keyDown(SDL_keysym*);
void keyUp(SDL_keysym*);
void mouseMotion(SDL_MouseMotionEvent*);
void reshape(int, int);
void display();
double findE(int, double, double, double);
void createUniverse();
void createTest();

void drawBody(CelestialBody);

CelestialBody * sun;
double t;
double t_factor;

double aspect = 1.0;

Camera camera;
SDL_Surface* surface;
int videoFlags;
bool forward, backward, left, right, up, down, cw, ccw, zin, zout;
bool mouseCap;

GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat yellow[] = { 1.0, 1.0, 0.55, 1.0 };
GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };

double findE(int n, double E, double M, double ecc) {

	double newE = E - ((E - ecc * sin(E) - M) / (1 - ecc * cos(E)));

	if (n == 0)
		return E;
	else
		return findE(n - 1, newE, M, ecc);

}

double findM(double per, double t) {
	return (2 * M_PI * t) / per;
}

double findTheta(double e, double E) {

	return 2 * atan(sqrt((1 + e) / (1 - e)) * tan(E / 2));

}

double findP(double a, double ecc) {
	return a * (1 - ecc) * (1 - ecc);
}

double findDist(double p, double ecc, double theta) {
	return p / (1 + ecc * cos(theta));
}

int main(int argc, char **argv) {
	initDisplay(argc, argv);
	init();
	initTextures();

	//createUniverse();
	createTest();

	mainLoop();
	return 0;
}

void initDisplay(int& argc, char** argv) {
	/* this holds some info about our display */
	const SDL_VideoInfo *videoInfo;

	/* initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Quit();
		exit(1);
	}

	/* Fetch the video info */
	videoInfo = SDL_GetVideoInfo();
	if (!videoInfo) {
		SDL_Quit();
		exit(1);
	}

	/* the flags to pass to SDL_SetVideoMode */
	videoFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE
			| SDL_RESIZABLE;

	/* This checks to see if surfaces can be stored in memory */
	if (videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;

	/* This checks if hardware blits can be done */
	if (videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

	/* Sets up OpenGL double buffering */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/* get a SDL surface */
	surface = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BPP, videoFlags);

	/* Verify there is a surface */
	if (!surface) {
		SDL_Quit();
		exit(1);
	}

	reshape(WINDOW_WIDTH, WINDOW_HEIGHT);

	SDL_WM_SetCaption("Solar System Sim", "Solar System Sim");

	mouseCap = true;
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(0);
}

void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_NORMALIZE);

	GLfloat pos[] = { 0.0, 0.0, 0.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	Point eye(20, 0, 0);
	Point lookAt(0, 0, 0);
	Vector up(0, 1, 0);

	camera.set(eye, lookAt, up, 90);
}

void initTextures() {
	Image* texture = new Image();
	texture->readFile("./textures/sun.bmp");
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/mercury.bmp");
	glBindTexture(GL_TEXTURE_2D, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/venus.bmp");
	glBindTexture(GL_TEXTURE_2D, 2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/earth.bmp");
	glBindTexture(GL_TEXTURE_2D, 3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/moon.bmp");
	glBindTexture(GL_TEXTURE_2D, 21);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/mars.bmp");
	glBindTexture(GL_TEXTURE_2D, 4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/jupiter.bmp");
	glBindTexture(GL_TEXTURE_2D, 5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/saturn.bmp");
	glBindTexture(GL_TEXTURE_2D, 6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/uranus.bmp");
	glBindTexture(GL_TEXTURE_2D, 7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/neptune.bmp");
	glBindTexture(GL_TEXTURE_2D, 8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/pluto.bmp");
	glBindTexture(GL_TEXTURE_2D, 9);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());
}

void mainLoop() {
	bool loop = true;
	SDL_Event event;
	forward = backward = left = right = up = down = cw = ccw = zin = zout
			= false;
	while (loop) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_VIDEORESIZE:
				surface = SDL_SetVideoMode(event.resize.w, event.resize.h, BPP,
				videoFlags);
				if (!surface)
					loop = false;
				reshape(event.resize.w, event.resize.h);
				break;
			case SDL_KEYDOWN:
				loop = keyDown(&event.key.keysym);
				break;
			case SDL_KEYUP:
				keyUp(&event.key.keysym);
				break;
			case SDL_MOUSEMOTION:
				if (mouseCap)
					mouseMotion(&event.motion);
				break;
			case SDL_QUIT:
				loop = false;
				break;
			}
		}

		double du = 0, dv = 0, dn = 0;
		if (forward)
			dn -= MOVE_SPEED;
		if (backward)
			dn += MOVE_SPEED;
		if (left)
			du -= MOVE_SPEED;
		if (right)
			du += MOVE_SPEED;
		if (ccw)
			camera.roll(-MOVE_SPEED);
		if (cw)
			camera.roll(MOVE_SPEED);
		if (up)
			dv += MOVE_SPEED;
		if (down)
			dv -= MOVE_SPEED;
		if (zin)
			camera.zoom(-ZOOM_SPEED);
		if (zout)
			camera.zoom(ZOOM_SPEED);

		camera.translate(du, dv, dn);

		t += t_factor;
		std::cout<<t_factor<<std::endl;

		display();
		SDL_GL_SwapBuffers();
	}

	SDL_Quit();
}

bool keyDown(SDL_keysym *keysym) {
	switch (keysym->sym) {
	case SDLK_F1:
		SDL_WM_ToggleFullScreen(surface);
		return true;
	case SDLK_w:
		return forward = true;
	case SDLK_s:
		return backward = true;
	case SDLK_a:
		return left = true;
	case SDLK_d:
		return right = true;
	case SDLK_q:
		return ccw = true;
	case SDLK_e:
		return cw = true;
	case SDLK_r:
		return zin = true;
	case SDLK_f:
		return zout = true;
	case SDLK_SPACE:
		return up = true;
	case SDLK_LCTRL:
		return down = true;
	case SDLK_ESCAPE:
		return false;
	case SDLK_TAB:
		mouseCap = !mouseCap;
		if (mouseCap) {
			SDL_WM_GrabInput(SDL_GRAB_ON);
			SDL_ShowCursor(0);
		} else {
			SDL_WM_GrabInput(SDL_GRAB_OFF);
			SDL_ShowCursor(1);
		}
		break;
	case SDLK_p:
		t_factor *= 10.0;
		break;
	case SDLK_MINUS:
		t_factor *= 0.1;
		break;
	default:
		return true;
	}
}

void keyUp(SDL_keysym *keysym) {
	switch (keysym->sym) {
	case SDLK_w:
		forward = false;
		break;
	case SDLK_s:
		backward = false;
		break;
	case SDLK_a:
		left = false;
		break;
	case SDLK_d:
		right = false;
		break;
	case SDLK_q:
		ccw = false;
		break;
	case SDLK_e:
		cw = false;
		break;
	case SDLK_r:
		zin = false;
		break;
	case SDLK_f:
		zout = false;
		break;
	case SDLK_SPACE:
		up = false;
		break;
	case SDLK_LCTRL:
		down = false;
		break;
	default:
		break;
	}
}

void mouseMotion(SDL_MouseMotionEvent *m) {
	camera.pitch((double) m->yrel * MOUSE_SENSITIVITY);
	camera.yaw((double) m->xrel * MOUSE_SENSITIVITY);
}

void reshape(int x, int y) {
	aspect = (double) x / (double) y;
	glViewport(0, 0, x, y);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera.getFOV(), aspect, 0.01, 50.0);
	Point pos = camera.getEye();
	Point dir = moveAlong(pos, invert(camera.getN()));
	Vector up = camera.getV();
	gluLookAt(pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, up.x, up.y, up.z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawBody(*sun);
	glFlush();
}

void createUniverse() {
	t = 0.0;
	t_factor = 0.1;

	Image* texture = new Image();
	texture->readFile("./textures/moon.bmp");
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	sun = new Planet(0, SUN_RADIUS, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	CelestialBody * mercury = new Planet(1, MERCURY_RADIUS*50, MERCURY_MAJOR,
	MERCURY_E, MERCURY_APH, MERCURY_PER, MERCURY_OMEGA, MERCURY_P);
	CelestialBody * venus = new Planet(2, VENUS_RADIUS*50, VENUS_MAJOR,
	VENUS_E, VENUS_APH, VENUS_PER, VENUS_OMEGA, VENUS_P);
	CelestialBody * earth = new Planet(3, EARTH_RADIUS*50, EARTH_MAJOR,
	EARTH_E, EARTH_APH, EARTH_PER, EARTH_OMEGA, EARTH_P);
	CelestialBody * moon = new Planet(1, MOON_RADIUS, MOON_MAJOR, MOON_E,
	MOON_APH, MOON_PER, MOON_OMEGA, MOON_P);
	earth->addChild(*moon);

	CelestialBody * mars = new Planet(4, MARS_RADIUS, MARS_MAJOR, MARS_E,
	MARS_APH, MARS_PER, MARS_OMEGA, MARS_P);
	CelestialBody * jupiter = new Planet(5, JUPITER_RADIUS,
	JUPITER_MAJOR, JUPITER_E, JUPITER_APH, JUPITER_PER,
	JUPITER_OMEGA, JUPITER_P);

	sun->addChild(*mercury);
	sun->addChild(*venus);
	sun->addChild(*earth);
	sun->addChild(*mars);
	sun->addChild(*jupiter);
}

void createTest() {
	t = 0.0;
	t_factor = 1.0;

	sun = new CelestialBody(0, 1.0, 1, 1, 1, 0, 0, 0);
	CelestialBody * child1 = new CelestialBody(3, 0.5, 5.0, 0.24880766,
			247.92065, 110.30347, 11.88, 113.76329);
	CelestialBody * child11 = new CelestialBody(21, 0.25, 1.0, 0.01671022,
			1.000174, 348.73936, 1.578694, 114.20783);
	child1->addChild(*child11);

	CelestialBody * child2 = new CelestialBody(9, 0.5, 9.0, 0.0549,
			0.074802414, 0, 5.145, 0);
	CelestialBody * child21 = new CelestialBody(6, 0.25, 1.0, 0.18874, 29.657,
			113.642811, 5.51, 336.013862);
	CelestialBody * child22 = new CelestialBody(5, 0.33, 1.75, 0.44177,
			11.8592, 100.492, 6.09, 275.066);
	child2->addChild(*child21);
	child2->addChild(*child22);

	sun->addChild(*child1);
	sun->addChild(*child2);
}

void drawBody(CelestialBody p) {
	/*
	 glPushMatrix();
	 GLUquadricObj * sph = gluNewQuadric();
	 gluQuadricNormals(sph, GLU_SMOOTH);
	 gluQuadricTexture(sph, GL_TRUE);

	 double pt = t / p.getPeriod();

	 double x = (p.getSemiMajor() * cos(pt * M_PI / 180.0) * cos(
	 p.getOmega() * M_PI / 180.0)) - (p.getSemiMajor() * sin(pt * M_PI
	 / 180.0) * sin(p.getOmega() * M_PI / 180.0));
	 double z = (p.getSemiMajor() * cos(pt * M_PI / 180.0) * sin(
	 p.getOmega() * M_PI / 180.0)) + (p.getSemiMajor() * sin(pt * M_PI
	 / 180.0) * cos(p.getOmega() * M_PI / 180.0));
	 glTranslated(x, 0, z);

	 glBindTexture(GL_TEXTURE_2D,p.getId());
	 if (p.getId() == 0)
	 glMaterialfv(GL_FRONT, GL_EMISSION, yellow);
	 gluSphere(sph, p.getRadius(), 100, 100);

	 if (p.getId() == 0)
	 glMaterialfv(GL_FRONT, GL_EMISSION, black);
	 if (int c = p.hasChildren()) {
	 for (int i = 0; i < c; i++) {
	 drawBody(p.getChild(i));
	 }
	 }
	 glPopMatrix();
	 */

	glPushMatrix();

	double x = 0;
	double z = 0;

	GLUquadricObj * sph = gluNewQuadric();
	gluQuadricNormals(sph, GLU_SMOOTH);
	gluQuadricTexture(sph, GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, p.getId());

	if (p.getId() == 0)
		glMaterialfv(GL_FRONT, GL_EMISSION, yellow);
	else {

		double a = p.getSemiMajor();
		double ecc = p.getEcc();
		double per = p.getPeriod();
		double lan = p.getOmega(); //Longitude of Ascending Node
		double apa = p.getW(); //Argument of Periapsis
		double i = p.getInc(); //inclination

		double slr = findP(a, ecc); //semi-latus rectum
		double M = findM(per, t); //Mean Anomoly
		double E = findE(NEWTON_DEPTH, NEWTON_GUESS, M, ecc);
		double theta = findTheta(ecc, E);
		double dist = findDist(slr, ecc, theta);

		x = dist * sin(theta);
		z = dist * -cos(theta);

		glRotated(apa, 0, 1, 0);//w
		glRotated(i, 1, 0, 0);//i
		glRotated(lan, 0, 1, 0);//omega
		glTranslated(x, 0, z);
		glRotated(-lan, 0, 1, 0);//omega
		glRotated(-i, 1, 0, 0);//i
		glRotated(-apa, 0, 1, 0);//w

		glPushMatrix();

	}

	//glRotated(360 * t / p.getOrbPer(), 0, 1, 0); //rotation as a function of time!
	//glRotated(90 + p.getATilt(), -1, 0, 0);//TODO Axial Tilt
	glRotated(90, -1, 0, 0);
	gluSphere(sph, p.getRadius(), 100, 100);
	glPopMatrix();

	if (p.getId() == 0)
		glMaterialfv(GL_FRONT, GL_EMISSION, yellow);

	if (int c = p.hasChildren()) {
		for (int i = 0; i < c; i++) {
			drawBody(p.getChild(i));
		}
	}

	glPopMatrix();

	/*glPushMatrix();

	 glLoadIdentity();

	 //SUN
	 GLUquadricObj * sphS = gluNewQuadric();
	 gluQuadricNormals(sphS, GLU_SMOOTH);
	 gluQuadricTexture(sphS, GL_TRUE);

	 glBindTexture(GL_TEXTURE_2D, 0);
	 glMaterialfv(GL_FRONT, GL_EMISSION, yellow);//TODO fix emissive lighting model

	 glPushMatrix();
	 glRotated(90, -1, 0, 0);
	 gluSphere(sphS, 2, 100, 100);
	 glPopMatrix();

	 glMaterialfv(GL_FRONT, GL_EMISSION, black);//TODO fix emissive lighting model

	 //Earth
	 glPushMatrix();

	 GLUquadricObj * sphE = gluNewQuadric();
	 gluQuadricNormals(sphE, GLU_SMOOTH);
	 gluQuadricTexture(sphE, GL_TRUE);
	 glBindTexture(GL_TEXTURE_2D, 3);

	 double eartha = 7.0;
	 double earthecc = 0.01671022;
	 double earthPer = 1.000174;
	 double slr = findP(eartha, earthecc);
	 double earthM = findM(earthPer, t);
	 double earthE = findE(NEWTON_DEPTH, NEWTON_GUESS, earthM, earthecc);
	 double earthTheta = findTheta(earthecc, earthE);

	 double earthr = findDist(slr, earthecc, earthTheta);

	 double earthapa = 114.20783;
	 double earthi = 1.578694;
	 double earthlan = 348.73936;

	 double x = earthr * sin(earthTheta);
	 double z = earthr * -cos(earthTheta);

	 glRotated(earthapa, 0, 1, 0);//w
	 glRotated(earthi, 1, 0, 0);//i
	 glRotated(earthlan, 0, 1, 0);//omega
	 glTranslated(x, 0, z);
	 glRotated(-earthlan, 0, 1, 0);//omega
	 glRotated(-earthi, 1, 0, 0);//i
	 glRotated(-earthapa, 0, 1, 0);//w

	 glPushMatrix();
	 glRotated(360*t/0.00273037557837098, 0 , 1, 0); //rotation as a function of time!
	 glRotated(113.439281, -1, 0, 0);//TODO Axial Tilt
	 gluSphere(sphE, 1, 100, 100);
	 glPopMatrix();

	 //Moon

	 GLUquadricObj * sphM = gluNewQuadric();
	 gluQuadricNormals(sphM, GLU_SMOOTH);
	 gluQuadricTexture(sphM, GL_TRUE);
	 glBindTexture(GL_TEXTURE_2D, 21);

	 double moona = 2;
	 double moonecc = 0.0549;
	 double moonper = 0.074802414;

	 double moonslr = findP(moona, moonecc);
	 double moonM = findM(moonper, t);
	 double moonE = findE(NEWTON_DEPTH, NEWTON_GUESS, moonM, moonecc);
	 double moonTheta = findTheta(moonecc, moonE);
	 double moonDist = findDist(moonslr, moonecc, moonTheta);

	 double moonlan = 0;
	 double mooni = 5.145;
	 double moonapa = 0;

	 x = moonDist * sin(moonTheta);
	 z = moonDist * -cos(moonTheta);

	 glRotated(moonapa, 0, 1, 0);//w
	 glRotated(mooni, 1, 0, 0);//i
	 glRotated(moonlan, 0, 1, 0);//omega
	 glTranslated(x, 0, z);
	 glRotated(-moonlan, 0, 1, 0);//omega
	 glRotated(-mooni, 1, 0, 0);//i
	 glRotated(-moonapa, 0, 1, 0);//w

	 glPushMatrix();
	 glRotated(91.5424, -1, 0, 0);//TODO Axial Tilt
	 gluSphere(sphM, 0.25, 100, 100);
	 glPopMatrix();

	 glPopMatrix();

	 //Pluto
	 glPushMatrix();

	 double plutoa = 30;
	 double plutoecc = 0.24880766;
	 double plutoPer = 247.92065;
	 double plutoP = findP(plutoa, plutoecc);
	 double plutoM = findM(plutoPer, t);
	 double plutoE = findE(NEWTON_DEPTH, NEWTON_GUESS, plutoM, plutoecc);
	 double plutoTheta = findTheta(plutoecc, plutoE);
	 double plutor = findDist(plutoP, plutoecc, plutoTheta);

	 x = plutor * sin(plutoTheta);
	 z = plutor * -cos(plutoTheta);

	 glRotated(113.76329, 0, 1, 0);//w
	 glRotated(11.88, 1, 0, 0);//i
	 glRotated(110.030347, 0, 1, 0);//omega
	 glTranslated(x, 0, z);

	 GLUquadricObj * sphP = gluNewQuadric();
	 gluQuadricNormals(sphP, GLU_SMOOTH);
	 gluQuadricTexture(sphP, GL_TRUE);

	 glBindTexture(GL_TEXTURE_2D, 9);

	 glPushMatrix();
	 glRotated(90, -1, 0, 0);
	 gluSphere(sphP, 0.35, 100, 100);
	 glPopMatrix();

	 glPopMatrix();

	 glPopMatrix();*/

}
