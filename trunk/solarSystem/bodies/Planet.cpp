#include "Planet.h"

namespace solarSystem {

Planet::Planet(std::string n, float r, float maj, float ex, float aph, float per,
		float om, float pd, float red, float gre, float blu) :
	CelestialBody(n, r, maj, ex, aph, per, om, pd, red, gre, blu) {
}

Planet::~Planet() {
}

}
