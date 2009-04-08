/*
 * Vector.cpp
 *
 *  Created on: 20-Feb-2009
 *      Author: brad
 */

#include "Vector.h"

using namespace std;

Vector::Vector() {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Vector::Vector(double xx, double yy, double zz) {
	x = xx;
	y = yy;
	z = zz;
}

void Vector::set(double xx, double yy, double zz) {
	x = xx;
	y = yy;
	z = zz;
}

void Vector::set(Vector v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

void Vector::setByDiff(Point a, Point b) {
	x = a.x - b.x;
	y = a.y - b.y;
	z = a.z - b.z;
}

void Vector::normalize() {
	double size = sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
	x = x / size;
	y = y / size;
	z = z / size;
}

void Vector::display() {
	cout << "x = " << x << ", y = " << y << ", z = " << z << endl;
}

Vector::~Vector() {
}

Vector invert(Vector v) {
	Vector r(-v.x, -v.y, -v.z);
	return r;
}

double dot(Vector s, Vector t) {
	return s.x * t.x + s.y * t.y + s.z * t.z;
}

Vector cross(Vector s, Vector t) {
	Vector * c = new Vector(s.y * t.z - s.z * t.y, s.z * t.x - s.x * t.z, s.x
			* t.y - s.y * t.x);
	return *c;
}

Point moveAlong(Point o, Vector d) {
	Point r(o.x + d.x, o.y + d.y, o.z + d.z);
	/*cout<<"o: ";o.display();
	cout<<"d: ";d.display();
	cout<<"r: ";r.display();*/
	return r;
}

double getMag(Vector d) {
	return sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
}

Vector scale(Vector v, double s) {
	Vector r(v.x * s, v.y * s, v.z * s);
	return r;
}
