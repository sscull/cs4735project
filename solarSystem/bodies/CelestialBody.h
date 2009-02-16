#ifndef CELESTIALBODY_H_
#define CELESTIALBODY_H_

#include <vector>
#include <cmath>

namespace solarSystem {
class CelestialBody;
}

namespace solarSystem {

class CelestialBody {
private:
	char* name;
	float radius;
	float orbitMajor;
	float e;
	std::vector<CelestialBody> children;
	float r;
	float g;
	float b;
public:
	CelestialBody(float, float, float, char*, float, float, float);
	virtual ~CelestialBody();
	float getRadius() const;
	float getMajor() const;
	float getMinor() const;
	char* getName() const;
	void addChild(const CelestialBody&);
	CelestialBody getChild(int) const;
	int hasChildren() const;
	float getRed() const;
	float getGreen() const;
	float getBlue() const;
};

}

#endif /* CELESTIALBODY_H_ */
