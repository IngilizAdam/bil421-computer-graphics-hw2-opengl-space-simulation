#pragma once

#include "Angel.h"
#include "gameobjects.h"

class Ring : public GameObject {

protected:
	float innerRadius;
	float outerRadius;
	int resolution;

public:
	Ring(vec3 position, float innerRadius, float outerRadius, vec4 color, int resolution, GLuint program);

	float getInnerRadius() { return innerRadius; }
	float getOuterRadius() { return outerRadius; }
	int getResolution() { return resolution; }
};
