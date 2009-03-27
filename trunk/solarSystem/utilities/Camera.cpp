/*
 * camera.cpp
 *
 *  Created on: 20-Feb-2009
 *      Author: brad
 */

#include "Camera.h"

namespace solarSystem{

Camera::Camera() {
	eye.set(0.0, 0.0, 0.0);
	u.set(1.0, 0.0, 0.0);
	v.set(0.0, 1.0, 0.0);
	n.set(0.0, 0.0, 1.0);
}

void Camera::set(Point newEye, Point lookAt, Vector up){
	eye.set(newEye);
	n.setByDiff(lookAt, newEye);
	n.normalize();
	u.set(cross(up, n));
	u.normalize();
	v.set(cross(n, v));
}

Vector Camera::getU(void){
	return u;
}

Vector Camera::getV(void){
	return v;
}

Vector Camera::getN(void){
	return n;
}

Point Camera::getEye(void){
	return eye;
}

void Camera::roll(double angle){
	double cosAng = cos(M_PI/180.0*angle);
	double sinAng = sin(M_PI/180.0*angle);

	Vector t(u);
	u.set(cosAng * t.x - sinAng * v.x, cosAng * t.y - sinAng * v.y, cosAng * t.z - sinAng * v.z);
	v.set(sinAng * t.x + cosAng * v.x, sinAng * t.y + cosAng * v.y, sinAng * t.z + sinAng * v.z);
}

void Camera::pitch(double angle){
	double cosAng = cos(M_PI/180.0*angle);
	double sinAng = sin(M_PI/180.0*angle);

	Vector t(v);
	v.set(cosAng * t.x - sinAng * n.x, cosAng * t.y - sinAng * n.y, cosAng * t.z - sinAng * n.z);
	n.set(sinAng * t.x + cosAng * n.x, sinAng * t.y + cosAng * n.y, sinAng * t.z + sinAng * n.z);
}

void Camera::yaw(double angle){
	double cosAng = cos(M_PI/180.0*angle);
	double sinAng = sin(M_PI/180.0*angle);

	Vector t(n);
	n.set(cosAng * t.x - sinAng * u.x, cosAng * t.y - sinAng * u.y, cosAng * t.z - sinAng * u.z);
	u.set(sinAng * t.x + cosAng * u.x, sinAng * t.y + cosAng * u.y, sinAng * t.z + sinAng * u.z);
}

void Camera::translate(double du, double dv, double dn){
	eye.x = eye.x + du * u.x + dv * v.x + dn * n.x;
	eye.y = eye.y + du * u.y + dv * v.y + dn * n.y;
	eye.z = eye.z + du * u.z + dv * v.z + dn * n.z;
}

Camera::~Camera() {
}

}
