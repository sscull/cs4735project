/*
 * Planet.h
 *
 *  Created on: Feb 2, 2009
 *      Author: p14q6
 */

#ifndef PLANET_H_
#define PLANET_H_

namespace solarSystem {
class Planet;
}

#include "CelestialBody.h"

namespace solarSystem {

class Planet: public CelestialBody {
public:
	Planet(std::string, float, float, float, float, float, float, float, float,
			float, float);
	virtual ~Planet();
};

}
#endif /* PLANET_H_ */
