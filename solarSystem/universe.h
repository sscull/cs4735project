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
#define MOVE_SPEED 0.1
#define MOUSE_SENSITIVITY 0.0003
#define ZOOM_SPEED 0.1
#define NEWTON_DEPTH 5
#define NEWTON_GUESS M_PI

#endif /* UNIVERSE_H_ */
