#ifndef WORLD_H
#define WORLD_H
#include "utilities.h"
// Property names of rigidbodies. This may expand in the future.
enum PROPERTY_NAMES
{
	PLAYER = 1,
	GROUND = 2,
	SEMISOLID
};

//1 rad x 180/pi = 57,296deg
const double RAD2DEG = 180 / M_PI; // Value to convert radians to degrees

#endif // !WORLD_H
