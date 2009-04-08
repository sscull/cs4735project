#include "constants.h"
#include "universe.h"

void drawBody(CelestialBody);

CelestialBody * sun;
double t;
double t_factor;

double aspect = 1.0;
double lock = true;
int planet = 5;
bool startCamera;

Camera camera;
SDL_Surface* surface;
int videoFlags;
bool forward, backward, left, right, up, down, cw, ccw, zin, zout, shift;
bool mouseCap;

GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat yellow[] = { 1.0, 1.0, 0.55, 1.0 };
GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };

Vector rotVecIntoOrbit(Vector r, double om, double i, double w) {

	double omRad = om * M_PI / 180.0;
	double iRad = i * M_PI / 180.0;
	double wRad = w * M_PI / 180.0;

	double c1 = cos(omRad);
	double c2 = cos(iRad);
	double c3 = cos(wRad);
	double s1 = sin(omRad);
	double s2 = sin(iRad);
	double s3 = sin(wRad);

	Vector m1(c1 * c3 - c2 * s1 * s3, s1 * s2, -c2 * c3 * s1 - c1 * s3);

	Vector m2(s2 * s3, c2, c3 * s2);

	Vector m3(c3 * s1 + c1 * c2 * s3, -c1 * s2, c1 * c2 * c3 - s1 * s3);

	Vector newR(dot(m1, r), dot(m2, r), dot(m3, r));

	return newR;

}

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

void goToPlanet(CelestialBody p) {

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

	double x = dist * sin(theta);
	double z = dist * -cos(theta);

	Vector r(x, 0, z);
	r = rotVecIntoOrbit(r, lan, i, apa);

	/*Point o(0, 0, 0); Flavor number 2
	 Point newEye = moveAlong(o, r);
	 r.normalize();
	 newEye = moveAlong(newEye, scale(r, 2*p.getRadius()));

	 camera.set(newEye, o, camera.getV(), camera.getFOV());*/

	Point o(0, 0, 0);
	Point look = moveAlong(o, r);
	Point newEye = moveAlong(look, scale(camera.getN(), p.getRadius() * 10));

	camera.set(newEye, look, camera.getV(), camera.getFOV());

}

int main(int argc, char **argv) {
	startCamera = false;
	initDisplay(argc, argv);
	init();
	initTextures();

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

	Point eye(660, 400, 0);
	Point lookAt(0, 0, 0);
	Vector up(0, 1, 0);
	/*Point eye(5, 0, 0); //TESTING FOR THE MOUSE STUFF
	 Point lookAt(0, 0, 0);
	 Vector up(0, 1, 0);*/

	camera.set(eye, lookAt, up, 70);

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
	glBindTexture(GL_TEXTURE_2D, 31);
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

	texture->readFile("./textures/jupiter_moons/io.bmp");
	glBindTexture(GL_TEXTURE_2D, 51);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/jupiter_moons/europa.bmp");
	glBindTexture(GL_TEXTURE_2D, 52);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/jupiter_moons/ganymede.bmp");
	glBindTexture(GL_TEXTURE_2D, 53);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/jupiter_moons/callisto.bmp");
	glBindTexture(GL_TEXTURE_2D, 54);
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

	texture->readFile("./textures/saturn/dione.bmp");
	glBindTexture(GL_TEXTURE_2D, 61);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/saturn/enceladus.bmp");
	glBindTexture(GL_TEXTURE_2D, 62);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/saturn/iapetus.bmp");
	glBindTexture(GL_TEXTURE_2D, 63);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/saturn/mimas.bmp");
	glBindTexture(GL_TEXTURE_2D, 64);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/saturn/rhea.bmp");
	glBindTexture(GL_TEXTURE_2D, 65);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/saturn/tethys.bmp");
	glBindTexture(GL_TEXTURE_2D, 66);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	texture->readFile("./textures/saturn/titan.bmp");
	glBindTexture(GL_TEXTURE_2D, 67);
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

	texture->readFile("./textures/starmap.bmp");
	glBindTexture(GL_TEXTURE_2D, 10);
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
			= shift = false;
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

		if (forward && shift)
			dn -= MOVE_SPEED * FASTER;
		else if (forward)
			dn -= MOVE_SPEED;
		if (backward && shift)
			dn += MOVE_SPEED * FASTER;
		else if (backward)
			dn += MOVE_SPEED;
		if (left && shift)
			du -= MOVE_SPEED * FASTER;
		else if (left)
			du -= MOVE_SPEED;
		if (right && shift)
			du += MOVE_SPEED * FASTER;
		else if (right)
			du += MOVE_SPEED;
		if (ccw)
			camera.roll(-ROT_SPEED);
		if (cw)
			camera.roll(ROT_SPEED);
		if (shift && up)
			dv += MOVE_SPEED * FASTER;
		else if (up)
			dv += MOVE_SPEED;
		if (shift && down)
			dv -= MOVE_SPEED * FASTER;
		else if (down)
			dv -= MOVE_SPEED;
		if (zin)
			camera.zoom(-ZOOM_SPEED);
		if (zout)
			camera.zoom(ZOOM_SPEED);

		camera.translate(du, dv, dn);

		t += t_factor;

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
	case SDLK_LSHIFT:
		return shift = true;
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
	case SDLK_EQUALS:
		t_factor *= 10.0;
		break;
	case SDLK_MINUS:
		t_factor *= 0.1;
		break;
	case SDLK_k:
		if (lock) {
			if (planet == 8)
				planet = 0;
			else
				planet++;
		}
		break;
	case SDLK_l:
		if (lock == true)
			lock = false;
		else
			lock = true;
		break;
	default:
		return true;
	}

	return true;
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
	case SDLK_LSHIFT:
		shift = false;
		break;
	default:
		break;
	}
}

void mouseMotion(SDL_MouseMotionEvent *m) {
	double dy = (double) m->yrel;
	double dx = (double) m->xrel;

	if (startCamera)
		camera.pitch(dy * MOUSE_SENSITIVITY);
	if (startCamera)
		camera.yaw(dx * MOUSE_SENSITIVITY);
}

void reshape(int x, int y) {
	aspect = (double) x / (double) y;
	glViewport(0, 0, x, y);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	Vector n = camera.getN();
	std::cout << "n: ";
	n.display();

	if (lock)
		goToPlanet(sun->getChild(planet));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera.getFOV(), aspect, 0.01, 20000.0);
	Point pos = camera.getEye();
	Point dir = moveAlong(pos, invert(camera.getN()));
	Vector up = camera.getV();

	gluLookAt(pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, up.x, up.y, up.z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	startCamera = true;

	glPushMatrix();

	GLUquadricObj * starmap = gluNewQuadric();
	gluQuadricNormals(starmap, GLU_SMOOTH);
	gluQuadricTexture(starmap, GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, 10);

	glRotated(-90, 1, 0, 0);
	gluSphere(starmap, 12500, 200, 200);

	glPopMatrix();

	drawBody(*sun);
	glFlush();
}

void createTest() {
	t = 0.0;
	t_factor = 0.00001;

	sun = new CelestialBody(SUN_ID, SUN_R, SUN_ROT_PER, SUN_ATILT,
			SUN_SEMI_MAJOR, SUN_ECC, SUN_PER, SUN_OM, SUN_INC, SUN_W);
	CelestialBody * child1 = new CelestialBody(MERCURY_ID, MERCURY_R,
			MERCURY_ROT_PER, MERCURY_ATILT, MERCURY_SEMI_MAJOR, MERCURY_ECC,
			MERCURY_PER, MERCURY_OM, MERCURY_INC, MERCURY_W);
	CelestialBody * child2 = new CelestialBody(VENUS_ID, VENUS_R,
			VENUS_ROT_PER, VENUS_ATILT, VENUS_SEMI_MAJOR, VENUS_ECC,
			VENUS_PER, VENUS_OM, VENUS_INC, VENUS_W);
	CelestialBody * child3 = new CelestialBody(EARTH_ID, EARTH_R,
			EARTH_ROT_PER, EARTH_ATILT, EARTH_SEMI_MAJOR, EARTH_ECC,
			EARTH_PER, EARTH_OM, EARTH_INC, EARTH_W);
	CelestialBody * child31 = new CelestialBody(MOON_ID, MOON_R, MOON_ROT_PER,
	MOON_ATILT, MOON_SEMI_MAJOR, MOON_ECC, MOON_PER, MOON_OM, MOON_INC, MOON_W);

	child3->addChild(*child31);

	CelestialBody * child4 = new CelestialBody(MARS_ID, MARS_R, MARS_ROT_PER,
	MARS_ATILT, MARS_SEMI_MAJOR, MARS_ECC, MARS_PER, MARS_OM, MARS_INC, MARS_W);

	CelestialBody * child5 = new CelestialBody(JUPITER_ID, JUPITER_R,
			JUPITER_ROT_PER, JUPITER_ATILT, JUPITER_SEMI_MAJOR, JUPITER_ECC,
			JUPITER_PER, JUPITER_OM, JUPITER_INC, JUPITER_W);
	CelestialBody * child51 = new CelestialBody(IO_ID, IO_R, IO_ROT_PER,
			IO_ATILT, IO_SEMI_MAJOR, IO_ECC, IO_PER, IO_OM, IO_INC, IO_W);
	CelestialBody * child52 = new CelestialBody(EUROPA_ID, EUROPA_R,
			EUROPA_ROT_PER, EUROPA_ATILT, EUROPA_SEMI_MAJOR, EUROPA_ECC,
			EUROPA_PER, EUROPA_OM, EUROPA_INC, EUROPA_W);
	CelestialBody * child53 = new CelestialBody(GANYMEDE_ID, GANYMEDE_R,
			GANYMEDE_ROT_PER, GANYMEDE_ATILT, GANYMEDE_SEMI_MAJOR,
			GANYMEDE_ECC, GANYMEDE_PER, GANYMEDE_OM, GANYMEDE_INC, GANYMEDE_W);
	CelestialBody * child54 = new CelestialBody(CALLISTO_ID, CALLISTO_R,
			CALLISTO_ROT_PER, CALLISTO_ATILT, CALLISTO_SEMI_MAJOR,
			CALLISTO_ECC, CALLISTO_PER, CALLISTO_OM, CALLISTO_INC, CALLISTO_W);
	child5->addChild(*child51);
	child5->addChild(*child52);
	child5->addChild(*child53);
	child5->addChild(*child54);

	CelestialBody * child6 = new CelestialBody(SATURN_ID, SATURN_R,
			SATURN_ROT_PER, SATURN_ATILT, SATURN_SEMI_MAJOR, SATURN_ECC,
			SATURN_PER, SATURN_OM, SATURN_INC, SATURN_W);
	CelestialBody * child61 = new CelestialBody(DIONE_ID, DIONE_R,
			DIONE_ROT_PER, DIONE_ATILT, DIONE_SEMI_MAJOR, DIONE_ECC, DIONE_PER,
			DIONE_OM, DIONE_INC, DIONE_W);
	CelestialBody * child62 = new CelestialBody(ENCELADUS_ID, ENCELADUS_R,
			ENCELADUS_ROT_PER, ENCELADUS_ATILT, ENCELADUS_SEMI_MAJOR,
			ENCELADUS_ECC, ENCELADUS_PER, ENCELADUS_OM, ENCELADUS_INC,
			ENCELADUS_W);
	CelestialBody * child63 = new CelestialBody(IAPETUS_ID, IAPETUS_R,
			IAPETUS_ROT_PER, IAPETUS_ATILT, IAPETUS_SEMI_MAJOR, IAPETUS_ECC,
			IAPETUS_PER, IAPETUS_OM, IAPETUS_INC, IAPETUS_W);
	CelestialBody * child64 = new CelestialBody(MIMAS_ID, MIMAS_R,
			MIMAS_ROT_PER, MIMAS_ATILT, MIMAS_SEMI_MAJOR, MIMAS_ECC, MIMAS_PER,
			MIMAS_OM, MIMAS_INC, MIMAS_W);
	CelestialBody * child65 = new CelestialBody(RHEA_ID, RHEA_R, RHEA_ROT_PER,
			RHEA_ATILT, RHEA_SEMI_MAJOR, RHEA_ECC, RHEA_PER, RHEA_OM, RHEA_INC,
			RHEA_W);
	CelestialBody * child66 = new CelestialBody(TETHYS_ID, TETHYS_R,
			TETHYS_ROT_PER, TETHYS_ATILT, TETHYS_SEMI_MAJOR, TETHYS_ECC,
			TETHYS_PER, TETHYS_OM, TETHYS_INC, TETHYS_W);
	CelestialBody * child67 = new CelestialBody(TITAN_ID, TITAN_R,
			TITAN_ROT_PER, TITAN_ATILT, TITAN_SEMI_MAJOR, TITAN_ECC, TITAN_PER,
			TITAN_OM, TITAN_INC, TITAN_W);
	child6->addChild(*child61);
	child6->addChild(*child62);
	child6->addChild(*child63);
	child6->addChild(*child64);
	child6->addChild(*child65);
	child6->addChild(*child66);
	child6->addChild(*child67);

	CelestialBody * child7 = new CelestialBody(URANUS_ID, URANUS_R,
			URANUS_ROT_PER, URANUS_ATILT, URANUS_SEMI_MAJOR, URANUS_ECC,
			URANUS_PER, URANUS_OM, URANUS_INC, URANUS_W);
	CelestialBody * child8 = new CelestialBody(NEPTUNE_ID, NEPTUNE_R,
			NEPTUNE_ROT_PER, NEPTUNE_ATILT, NEPTUNE_SEMI_MAJOR, NEPTUNE_ECC,
			NEPTUNE_PER, NEPTUNE_OM, NEPTUNE_INC, NEPTUNE_W);
	CelestialBody * child9 = new CelestialBody(PLUTO_ID, PLUTO_R,
			PLUTO_ROT_PER, PLUTO_ATILT, PLUTO_SEMI_MAJOR, PLUTO_ECC,
			PLUTO_PER, PLUTO_OM, PLUTO_INC, PLUTO_W);

	sun->addChild(*child1);
	sun->addChild(*child2);
	sun->addChild(*child3);
	sun->addChild(*child4);
	sun->addChild(*child5);
	sun->addChild(*child6);
	sun->addChild(*child7);
	sun->addChild(*child8);
	sun->addChild(*child9);
}

void drawBody(CelestialBody p) {

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

		glMaterialfv(GL_FRONT, GL_EMISSION, black);

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

		Vector r(x, 0, z);
		r = rotVecIntoOrbit(r, lan, i, apa);

		glTranslated(r.x, r.y, r.z);

		glPushMatrix();

	}
	glRotated(p.getATilt(), 1, 0, 0);
	glRotated(360 * t / p.getRotPer(), 0, 1, 0); //rotation as a function of time!
	if (p.getId() == 6) {
		for (int i = 0; i < 1000; i++) {

			double randPi = rand() % 360;
			double randR = (rand() % (int) 100 * p.getRadius()) / 100.0 + 2
					* p.getRadius();
			std::cout << randPi << std::endl;
			std::cout << randR << std::endl;

			glBegin(GL_POINTS);
			glVertex3f(randR * cos(randPi * M_PI / 180), 0, randR * sin(randPi
					* M_PI / 180));
			glEnd();
		}
	}
	glRotated(-90, 1, 0, 0);

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

}
