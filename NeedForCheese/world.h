

#ifndef WORLD_H
#define WORLD_H

typedef enum PROPERTY_NAMES
{
	PLAYER = 1,
	GROUND = 2
};

const int WIDTH = 1280;
const int HEIGHT = 720;

const int SCALED_WIDTH = WIDTH / MET2PIX; // 4 | 3
const int SCALED_HEIGHT = HEIGHT / MET2PIX;

//1 rad x 180/pi = 57,296deg
const float RAD2DEG = 180 / M_PI;

#endif // !WORLD_H
