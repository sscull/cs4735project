/*
 * Point.cpp
 *
 *  Created on: Feb 16, 2009
 *      Author: p14q6
 */

#include "Point.h"

using namespace std;

Point::Point() {
	x = 0.0, y = 0.0, z = 0.0;
}

Point::Point(double xx, double yy, double zz) {
	x = xx, y = yy, z = zz;
}

void Point::set(double xx, double yy, double zz){
	x = xx, y = yy, z = zz;
}

void Point::set(Point p){
	x = p.x;
	y = p.y;
	z = p.z;
}

void Point::buildTuple(double v[]){
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = 1.0;
}

Point::~Point() {
}
