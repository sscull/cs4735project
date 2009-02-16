#include "Planet.h"

namespace solarSystem {

Planet::Planet(float r, float oMaj, float ex, char* n, float red, float green, float blue) :
	CelestialBody(r, oMaj, ex, n, red, green, blue) {
}

Planet::~Planet() {
}

}
