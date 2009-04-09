#include "CelestialBody.h"

namespace solarSystem {

CelestialBody::CelestialBody(int n, float r, float rp, float at, float maj, float e, float per,
		float om, float i, float w) {
	//textures
	id = n;

	//planet properties
	radius = r;
	rotPer = rp;
	aTilt = at;

	//orbital properites
	semiMaj = maj;
	ecc = e;
	period = per;
	omega = om;
	inc = i;
	apa = w;
}

CelestialBody::~CelestialBody() {
}

int CelestialBody::getId() const {
	return id;
}

void CelestialBody::addChild(const CelestialBody& c) {
	children.push_back(c);
}

CelestialBody CelestialBody::getChild(int c) const {
	return children[c];
}

int CelestialBody::hasChildren() const {
	return children.size();
}

//*****Planet Methods

float CelestialBody::getRadius() const {
	return radius;
}

float CelestialBody::getRotPer() const {
	return rotPer;
}

float CelestialBody::getATilt() const {
	return aTilt;
}

//*****Oribital Methods

float CelestialBody::getSemiMajor() const {
	return semiMaj;
}

float CelestialBody::getEcc() const {
	return ecc;
}

float CelestialBody::getPeriod() const {
	return period;
}

float CelestialBody::getOmega() const {
	return omega;
}

float CelestialBody::getW() const {
	return apa;
}

float CelestialBody::getInc() const {
	return inc;
}

}
