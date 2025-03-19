#pragma once

#include "Angel.h"
#include "gameobjects.h"

class Spaceship : public GameObject {

protected:
	float radius;
	int resolution;
	
public:
	Spaceship(vec3 position, float radius, float thickness, vec4 color, int resolution, GLuint program);

	void updateExtra() override;

	float getRadius() { return radius; }
	int getResolution() { return resolution; }
};