/*
 * camera.h
 *
 *  Created on: 20-Feb-2009
 *      Author: brad
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vector.h"

class Camera {
private:
	Point eye;
	Vector u, v, n;
	double fov;
public:
	Camera();
	void set(Point, Point, Vector, double);
	Vector getU(void);
	Vector getV(void);
	Vector getN(void);
	Point getEye(void);
	double getFOV(void);
	void roll(double);
	void pitch(double);
	void yaw(double);
	void translate(double, double, double);
	void zoom(double);
	virtual ~Camera();
};

#endif /* CAMERA_H_ */
