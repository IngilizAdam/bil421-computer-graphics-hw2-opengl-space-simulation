#include "planet.h"
#include "first.h"

// implement constructor of Planet class
Planet::Planet(vec3 position, vec3 rotation, float radius, vec4 color, int resolution, GLuint program) : GameObject(program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
	this->rotation = vec4(rotation.x, rotation.y, rotation.z, 0.0f);
    this->radius = radius;
	this->color = color;
	this->resolution = resolution;
	this->program = program;

	Mesh* sphere = Mesh::drawSphere(radius, resolution, color);
	sphere->setParentPosition(this->position);
	sphere->setParentRotation(this->rotation);
	sphere->setParentScale(vec3(GENERAL_SCALE, GENERAL_SCALE, GENERAL_SCALE));
	meshs.push_back(sphere);

	Mesh* ring = Mesh::drawTorus(radius + 1.0f, radius + 3.0f, resolution, vec4(0.6, 0.6, 0.6, 1.0));
	ring->setParentPosition(this->position);
	ring->setParentRotation(this->rotation);
	ring->setParentScale(vec3(GENERAL_SCALE, GENERAL_SCALE, GENERAL_SCALE));
	meshs.push_back(ring);

	setupBuffers();
}