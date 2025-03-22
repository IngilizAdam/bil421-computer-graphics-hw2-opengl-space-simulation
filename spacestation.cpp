#include "spacestation.h"
#include "first.h"

SpaceStation::SpaceStation(vec3 position, float radius, vec4 color, int resolution, GLuint program) : GameObject(program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
	this->rotation = vec4(0, 0, 0, 0);
	this->color = color;
	this->resolution = resolution;
	this->radius = radius;

	Mesh* sphere = Mesh::drawSphere(radius, resolution, color);
	sphere->setScale(vec3(2));
	sphere->setParentPosition(this->position);
	sphere->setParentRotation(this->rotation);
	sphere->setParentScale(vec3(GENERAL_SCALE, GENERAL_SCALE, GENERAL_SCALE));
	meshs.push_back(sphere);

	Mesh* tetrahedron = Mesh::drawTetrahedron(1, 2, color);
	tetrahedron->setPosition(vec3(0, 2*radius, 0));
	tetrahedron->setRotation(vec3(-90, 0, 0));
	tetrahedron->setScale(vec3(3, 3, 5));
	tetrahedron->setParentPosition(this->position);
	tetrahedron->setParentRotation(this->rotation);
	tetrahedron->setParentScale(vec3(GENERAL_SCALE, GENERAL_SCALE, GENERAL_SCALE));
	meshs.push_back(tetrahedron);

	setupBuffers();
}