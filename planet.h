#pragma once

#include "Angel.h"
#include "gameobjects.h"
#include <vector>

// initialize the class for planet objects
class Planet : public GameObject {
	float radius;
	int resolution;

public:
	// constructor
	Planet(vec3 position, float radius, vec4 color, int resolution, GLuint program);

	// getters
	float getRadius() { return radius; }
	int getResolution() { return resolution; }
};