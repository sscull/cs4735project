#ifndef CELESTIALBODY_H_
#define CELESTIALBODY_H_

#include <vector>
#include <cmath>
#include <string>

#include "../utilities/Point.h"

namespace solarSystem {
class CelestialBody;
}

namespace solarSystem {

class CelestialBody {
private:
	int id;
	std::vector<CelestialBody> children;

	//planet
	float radius;
	float orbPer;
	float aTilt;

	//orbital
	float semiMaj;
	float ecc;
	float period;
	float omega; // longitude of ascending node
	float inc;
	float apa;

public:
	CelestialBody(int, float, float, float, float, float, float, float);
	virtual ~CelestialBody();
	int getId() const;
	void addChild(const CelestialBody&);
	CelestialBody getChild(int) const;
	int hasChildren() const;
	//*****
	float getRadius() const;
	//*****
	float getSemiMajor() const;
	float getEcc() const;
	float getPeriod() const;
	float getOmega() const;
	float getW() const;
	float getInc() const;
};

}

#endif /* CELESTIALBODY_H_ */
