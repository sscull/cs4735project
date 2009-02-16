/*
 * Point.cpp
 *
 *  Created on: Feb 16, 2009
 *      Author: p14q6
 */

#include "Point.h"

Point::Point() {
	x = 0.0, y = 0.0, z = 0.0;
}

Point::Point(float xx, float yy, float zz) {
	x = xx, y = yy, z = zz;
}

Point::~Point() {
}
