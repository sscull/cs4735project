/*
 * Vector.cpp
 *
 *  Created on: 20-Feb-2009
 *      Author: brad
 */

#include "Vector.h" //FINISH THIS

Vector::Vector() {
	x = 0.0; y = 0.0; z = 0.0;
}

Vector::Vector(double xx, double yy, double zz){
	x = xx; y = yy; z = zz;
}

void Vector::set(double xx, double yy, double zz){
	x = xx; y = yy; z = zz;
}

void Vector::set(Vector v){
	x = v.x; y = v.y; z = v.z;
}

void Vector::invert(){
	x = -x; y = -y; z = -z;
}

void Vector::setByDiff(Point a, Point b){
	x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;
}

void Vector::normalize(){
	double size = sqrt(pow(x,2.0) + pow(y,2.0) + pow(z,2.0));
	x = x/size;
	y = y/size;
	z = z/size;
}

Vector::~Vector() {
}

double dot(Vector s, Vector t){
	return s.x * t.x + s.y * t.y + s.z * t.z;
}

Vector cross(Vector s, Vector t){
	Vector * c = new Vector(s.y*t.z - s.z*t.y, s.z*t.x - s.x*t.z, s.x*t.y - s.y*t.x);
	return *c;
}

