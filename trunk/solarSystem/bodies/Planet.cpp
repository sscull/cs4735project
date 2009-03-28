#include "Planet.h"

namespace solarSystem {

Planet::Planet(int n, float r, float maj, float ex, float aph, float per,
		float om, float pd) :
	CelestialBody(n, r, maj, ex, aph, per, om, pd) {
}

Planet::~Planet() {
}

}
