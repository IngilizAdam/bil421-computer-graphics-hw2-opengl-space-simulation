#include "spaceship.h"

// implement constructor of Spaceship class
Spaceship::Spaceship(vec3 position, float radius, vec4 color, int resolution, GLuint program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
	this->radius = radius;
	this->color = color;
	this->resolution = resolution;
	this->program = program;

	Mesh mesh = drawSphere(radius, resolution, color);
	mesh.setPosition(this->position);

	setupBuffers();
}

void Spaceship::updateExtra() {
	
}