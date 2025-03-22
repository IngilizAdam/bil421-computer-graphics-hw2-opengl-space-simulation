#pragma once

#include "Angel.h"
#include "gameobjects.h"

class SpaceStation : public GameObject {
	int resolution;
	float radius;

public:
	SpaceStation(vec3 position, float radius, vec4 color, int resolution, GLuint program);

	int getResolution() { return resolution; }
	float getRadius() { return radius; }
};