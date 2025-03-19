#pragma once

#include "Angel.h"
#include "gameobjects.h"

class Spaceship : public GameObject {

protected:
	float radius;
	int resolution;
	
public:
	Spaceship(vec3 position, vec3 rotation, float radius, float thickness, vec4 color, int resolution, GLuint program);

	float getRadius() { return radius; }
	int getResolution() { return resolution; }
};