#include "constants.h"
#include "universe.h"

using namespace solarSystem;
void mainLoop();
void init();
void initDisplay(int&, char**);
bool keyDown(SDL_keysym*);
void keyUp(SDL_keysym*);
void mouseMotion(SDL_MouseMotionEvent*);
void reshape(int, int);
void display();

void createUniverse();
void createTest();

void drawBody(CelestialBody);

CelestialBody * sun;
double t;

double aspect = 1.0;

Camera camera;
SDL_Surface* surface;
int videoFlags;
bool forward, backward, left, right, up, down, cw, ccw, zin, zout;
bool mouseCap;

GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };

int main(int argc, char **argv) {
	initDisplay(argc, argv);
	init();

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

	Point eye(5, 0, 0);
	Point lookAt(0, 0, 0);
	Vector up(0, 1, 0);

	camera.set(eye, lookAt, up, 90);
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

		t += 0.1;

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
	CelestialBody * moon = new Planet(31, MOON_RADIUS, MOON_MAJOR, MOON_E,
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

	Image* texture = new Image();
	texture->readFile("./textures/earth.bmp");
	std::cout << texture->getWidth() << " " << texture->getHeight()
			<< std::endl;
	glBindTexture(GL_TEXTURE_2D, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
	texture->getWidth(), texture->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
			texture->getData());

	sun = new Planet(0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	CelestialBody * child1 = new Planet(1, 0.5, 5.0, 0.5, 1.0, 1.0, 180.0, 1.0);
	CelestialBody * child11 =
			new Planet(11, 0.25, 1.0, 0.0, 1.0, 1.0, 0.0, 0.5);
	child1->addChild(*child11);

	CelestialBody * child2 =
			new Planet(2, 0.5, 9.0, 0.01, 1.0, 1.0, 180.0, 2.0);
	CelestialBody * child21 = new Planet(21, 0.25, 1.0, 0.5, 1.0, 1.0, 180.0,
			0.1);
	CelestialBody * child22 = new Planet(22, 0.33, 1.75, 0.5, 1.0, 1.0, 180.0,
			0.2);
	child2->addChild(*child21);
	child2->addChild(*child22);

	sun->addChild(*child1);
	sun->addChild(*child2);
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
	double z = p.getCenter().z + (p.getMajor() * cos(pt * M_PI / 180.0) * sin(
			p.getOmega() * M_PI / 180.0)) + (p.getMinor() * sin(pt * M_PI
			/ 180.0) * cos(p.getOmega() * M_PI / 180.0));
	glTranslated(x, 0, z);
	glBindTexture(GL_TEXTURE_2D,p.getId());
	gluSphere(sph, p.getRadius(), 100, 100);
	if (int c = p.hasChildren()) {
		for (int i = 0; i < c; i++) {
			drawBody(p.getChild(i));
		}
	}
	glPopMatrix();
}
