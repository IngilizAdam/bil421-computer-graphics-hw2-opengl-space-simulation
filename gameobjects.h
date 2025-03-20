#pragma once

#include "Angel.h"
#include <vector>
#include "mesh.h"

class GameObject {

protected:
	// general
	vec4 position;
	vec4 rotation;
	vec3 scale;
	vec4 color;

	// physics
	vec4 velocity;

	// drawables
	std::vector<Mesh*> meshs;

	// opengl stuff
	std::vector<GLuint> vao;
	std::vector<GLuint> buffer;
	GLuint program;
	GLuint model;

public:
	GameObject(GLuint program) {
		this->position = vec4(0.0, 0.0, 0.0, 1.0);
		this->rotation = vec4(0.0, 0.0, 0.0, 0.0);
		this->scale = vec3(1.0, 1.0, 1.0);
		this->velocity = vec4(0.0, 0.0, 0.0, 0.0);
		this->program = program;
		this->model = glGetUniformLocation(program, "model");		
	}

	void setupBuffers();
	void updateBuffers();
	void updatePhysics();

	void setPosition(vec4 position);
	void setRotation(vec4 rotation);
	void setVelocity(vec4 velocity);

	vec4 getPosition() { return position; }
	vec4 getRotation() { return rotation; }
	vec4 getVelocity() { return velocity; }
};
