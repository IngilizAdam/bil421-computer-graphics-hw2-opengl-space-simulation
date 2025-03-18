#include "ring.h"

// implement constructor of Ring class
Ring::Ring(vec3 position, float innerRadius, float outerRadius, vec4 color, int resolution, GLuint program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
	this->innerRadius = innerRadius;
	this->outerRadius = outerRadius;
	this->color = color;
	this->resolution = resolution;
	this->program = program;

	mesh = drawTorus(innerRadius, outerRadius, resolution, color, position);

	setupBuffers();
}