/*
 * Point.h
 *
 *  Created on: Feb 16, 2009
 *      Author: p14q6
 */

#ifndef POINT_H_
#define POINT_H_

class Point {
public:
	Point();
	Point(float, float, float);
	virtual ~Point();
	float x;
	float y;
	float z;
};

#endif /* POINT_H_ */
