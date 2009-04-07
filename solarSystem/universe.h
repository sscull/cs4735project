/*
 * universe.h
 *
 *  Created on: Feb 11, 2009
 *      Author: p14q6
 */

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <iostream>
#include <cmath>

#include "./bodies/CelestialBody.h"
#include "./utilities/Point.h"
#include "./utilities/Vector.h"
#include "./utilities/Camera.h"
#include "./utilities/Image.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800
#define BPP 24
#define MOVE_SPEED 1.0
#define FASTER 10
#define ROT_SPEED 0.001
#define MOUSE_SENSITIVITY 0.0003
#define ZOOM_SPEED 0.1
#define NEWTON_DEPTH 5
#define NEWTON_GUESS M_PI

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
void createTest();

#endif /* UNIVERSE_H_ */
