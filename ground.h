#pragma once

#include "Angel.h"
#include "gameobjects.h"

class Ground : public GameObject {
	int subdivision;
	float width;
	float height;

public:
	Ground(vec3 position, int subdivision, float width, float height, vec4 color, GLuint program);
};