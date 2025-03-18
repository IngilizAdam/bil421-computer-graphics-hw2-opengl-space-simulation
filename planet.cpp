#include "planet.h"

// implement constructor of Planet class
Planet::Planet(vec3 position, float radius, vec4 color, int resolution, GLuint program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
    this->radius = radius;
	this->color = color;
	this->resolution = resolution;
	this->program = program;

	mesh = drawSphere(radius, resolution, color, position);

	setupBuffers();
}