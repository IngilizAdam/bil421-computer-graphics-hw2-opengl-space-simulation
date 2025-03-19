#include "planet.h"

// implement constructor of Planet class
Planet::Planet(vec3 position, vec3 rotation, float radius, vec4 color, int resolution, GLuint program) : GameObject(program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
	this->rotation = vec4(rotation.x, rotation.y, rotation.z, 0.0f);
    this->radius = radius;
	this->color = color;
	this->resolution = resolution;
	this->program = program;

	Mesh sphere = drawSphere(radius, resolution, color);
	sphere.setParentPosition(this->position);
	sphere.setParentRotation(this->rotation);
	meshs.push_back(sphere);

	Mesh ring = drawTorus(radius + 0.1f, radius + 0.12f, resolution, vec4(1.0, 1.0, 1.0, 1.0));
	ring.setParentPosition(this->position);
	ring.setParentRotation(this->rotation);
	meshs.push_back(ring);

	setupBuffers();
}