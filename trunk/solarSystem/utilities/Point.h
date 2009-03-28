/*
 * Point.h
 *
 *  Created on: Feb 16, 2009
 *      Author: p14q6
 */

#ifndef POINT_H_
#define POINT_H_

#include <iostream>

class Point {
public:
	Point();
	Point(double, double, double);
	void set(double, double, double);
	void set(Point);
	void buildTuple(double []);
	void display(void);
	virtual ~Point();
	double x;
	double y;
	double z;
};

#endif /* POINT_H_ */
