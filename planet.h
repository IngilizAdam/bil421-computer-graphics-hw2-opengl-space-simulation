#pragma once

#include "Angel.h"
#include <vector>
#include "gameobjects.h"

// initialize the class for planet objects
class Planet : public GameObject {

protected:
	float radius;
	int resolution;

public:
	// constructor
	Planet(vec3 position, vec3 rotation, float radius, vec4 color, int resolution, GLuint program);

	void updateExtra() override;

	// getters
	float getRadius() { return radius; }
	int getResolution() { return resolution; }
};