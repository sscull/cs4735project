/*
 * Vector.h
 *
 *  Created on: 20-Feb-2009
 *      Author: brad
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include<cmath>
#include"Point.h"

class Vector {
public:
	Vector();
	Vector(double, double, double);
	void set(double, double, double);
	void set(Vector);
	void invert();
	void setByDiff(Point, Point);
	void normalize();
	virtual ~Vector();
	double x;
	double y;
	double z;
};

double dot(Vector, Vector);
Vector cross(Vector, Vector);

#endif /* VECTOR_H_ */
