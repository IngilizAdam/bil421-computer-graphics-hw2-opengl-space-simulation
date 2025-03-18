#include "planet.h"

// implement constructor of Planet class
Planet::Planet(vec3 position, float radius, vec4 color, int resolution, GLuint program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
    this->radius = radius;
	this->color = color;
	this->resolution = resolution;
	this->program = program;

	Mesh sphere = drawSphere(radius, resolution, color);
	sphere.setPosition(this->position);
	meshs.push_back(sphere);

	Mesh ring = drawTorus(radius + 0.1f, radius + 0.12f, resolution, vec4(1.0, 1.0, 1.0, 1.0));
	ring.setPosition(this->position);
	meshs.push_back(ring);

	setupBuffers();
}

void Planet::updateExtra() {
	
}