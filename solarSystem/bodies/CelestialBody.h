#ifndef CELESTIALBODY_H_
#define CELESTIALBODY_H_

#include <vector>
#include <cmath>

#include "Point.h"

namespace solarSystem {
class CelestialBody;
}

namespace solarSystem {

class CelestialBody {
private:
	char* name;
	std::vector<CelestialBody> children;
	float radius;
	float major;
	float minor;
	Point* oCenter; // center of elliptical orbit
	float omega; // longitude of ascending node
	float period;

	float red;
	float green;
	float blue;

public:
	CelestialBody(char*, float, float, float, float, float, float, float,
			float, float, float);
	virtual ~CelestialBody();
	char* getName() const;
	void addChild(const CelestialBody&);
	CelestialBody getChild(int) const;
	int hasChildren() const;
	float getRadius() const;
	float getMajor() const;
	float getMinor() const;
	Point getCenter() const;
	void setCenter(Point);
	float getOmega() const;
	float getPeriod() const;
	float getRed() const;
	float getGreen() const;
	float getBlue() const;
};

}

#endif /* CELESTIALBODY_H_ */
