#ifndef WORLD_H
#define WORLD_H

// Property names of rigidbodies. This may expand in the future.
enum PROPERTY_NAMES
{
	PLAYER = 1,
	GROUND = 2
};

const int WIDTH = 1280; // The width of the window, in pixels.
const int HEIGHT = 720; // The height of the window, in pixels.
static int WIDTH = 1280; // The width of the window, in pixels.
static int HEIGHT = 720; // The height of the window, in pixels.

const int SCALED_WIDTH = WIDTH / MET2PIX; // The width of the window, in meters.
const int SCALED_HEIGHT = HEIGHT / MET2PIX; // The height of the window, in meters.

//1 rad x 180/pi = 57,296deg
const float RAD2DEG = 180 / M_PI; // Value to convert radians to degrees

#endif // !WORLD_H
