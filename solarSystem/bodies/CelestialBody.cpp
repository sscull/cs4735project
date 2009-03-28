#include "CelestialBody.h"

namespace solarSystem {

CelestialBody::CelestialBody(std::string n, float r, float maj, float ex,
		float aph, float per, float om, float pd) {
	name = n;
	radius = r;
	major = maj;
	minor = maj * sqrt(1 - pow(ex, 2));
	float c = (aph - per) / 2.0;
	// no orbital tilt
	oCenter
			= new Point(c * cos(om * M_PI / 180), 0.0, c * sin(om * M_PI / 180));
	omega = om;
	period = pd;
}

CelestialBody::~CelestialBody() {
}

std::string CelestialBody::getName() const {
	return name;
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

float CelestialBody::getRadius() const {
	return radius;
}

float CelestialBody::getMajor() const {
	return major;
}

float CelestialBody::getMinor() const {
	return minor;
}

Point CelestialBody::getCenter() const {
	return *oCenter;
}

void CelestialBody::setCenter(Point c) {
	oCenter->x = c.x;
	oCenter->y = c.y;
	oCenter->z = c.z;
}

float CelestialBody::getOmega() const {
	return omega;
}

float CelestialBody::getPeriod() const {
	return period;
}

}
