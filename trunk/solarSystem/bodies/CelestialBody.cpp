#include "CelestialBody.h"

namespace solarSystem {

CelestialBody::CelestialBody(float rad, float oMaj, float ex, char* n, float red, float green, float blue) {
	radius = rad;
	orbitMajor = oMaj;
	e = ex;
	name = n;
	r = red;
	g = green;
	b = blue;
}

CelestialBody::~CelestialBody() {
}

float CelestialBody::getRadius() const {
	return radius;
}

float CelestialBody::getMajor() const {
	return orbitMajor;
}

float CelestialBody::getMinor() const {
	return orbitMajor * sqrt(1 - pow(e, 2));
}

void CelestialBody::addChild(const CelestialBody& c) {
	children.push_back(c);
}

char* CelestialBody::getName() const {
	return name;
}

CelestialBody CelestialBody::getChild(int c) const {
	return children[c];
}

int CelestialBody::hasChildren() const {
	return children.size();
}

float CelestialBody::getRed() const {
	return r;
}

float CelestialBody::getBlue() const {
	return b;
}

float CelestialBody::getGreen() const {
	return g;
}

}
