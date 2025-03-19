#include "spaceship.h"

// implement constructor of Spaceship class
Spaceship::Spaceship(vec3 position, float radius, float thickness, vec4 color, int resolution, GLuint program) : GameObject(program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
	this->radius = radius;
	this->color = color;
	this->resolution = resolution;
	this->program = program;

	Mesh torus1 = drawTorus(radius, radius + thickness, resolution, color);
	torus1.setPosition(this->position);
	meshs.push_back(torus1);

	Mesh torus2 = drawTorus(radius, radius + thickness, resolution, color);
	torus2.setRotation(vec3(0, 90, 0));
	torus2.setPosition(this->position);
	meshs.push_back(torus2);

	setupBuffers();
}

void Spaceship::updateExtra() {
	
}