#include "planet.h"

// implement constructor of Planet class
Planet::Planet(vec3 position, float radius, vec4 color, int resolution, GLuint program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
    this->radius = radius;
	this->color = color;
	this->resolution = resolution;
	this->program = program;

	mesh = drawSphere(radius, resolution, color, position);

	Ring ring1 = Ring(position, radius + 1.0f, radius + 1.2f, vec4(0.0, 1.0, 1.0, 1.0), 40, program);
	rings.push_back(ring1);

	setupBuffers();
}

void Planet::updateExtra() {
	for (int i = 0; i < rings.size(); i++) {
		rings[i].updateBuffers();
	}
}