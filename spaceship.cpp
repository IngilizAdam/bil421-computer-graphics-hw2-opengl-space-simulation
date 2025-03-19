#include "spaceship.h"
#include "first.h"

// implement constructor of Spaceship class
Spaceship::Spaceship(vec3 position, vec3 rotation, float radius, float thickness, vec4 color, int resolution, GLuint program) : GameObject(program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
	this->rotation = vec4(rotation.x, rotation.y, rotation.z, 0.0f);
	this->radius = radius;
	this->color = color;
	this->resolution = resolution;

	Mesh torus1 = drawTorus(radius, radius + thickness, resolution, color);
	torus1.setScale(vec3(0.7, 1.0, 1.0));
	torus1.setParentPosition(this->position);
	torus1.setParentRotation(this->rotation);
	torus1.setParentScale(vec3(GENERAL_SCALE, GENERAL_SCALE, GENERAL_SCALE));
	meshs.push_back(torus1);

	Mesh torus2 = drawTorus(radius, radius + thickness, resolution, color);
	torus2.setRotation(vec3(0, 90, 0));
	torus2.setScale(vec3(0.7, 1.0, 1.0));
	torus2.setParentPosition(this->position);
	torus2.setParentRotation(this->rotation);
	torus2.setParentScale(vec3(GENERAL_SCALE, GENERAL_SCALE, GENERAL_SCALE));
	meshs.push_back(torus2);

	setupBuffers();
}