#pragma once

#include "Angel.h"
#include <vector>
#include "mesh.h"

class GameObject {

protected:
	vec4 position;
	vec4 rotation;
	vec4 color;

	std::vector<Mesh> meshs;

	std::vector<GLuint> vao;
	std::vector<GLuint> buffer;
	GLuint program;

public:
	GameObject() {}

	GameObject(GLuint program) {
		this->program = program;
		setupBuffers();
	}

	void setupBuffers();
	void updateBuffers();
	virtual void updateExtra();

	static Mesh drawSphere(float radius, int resolution, vec4 color);
	static Mesh drawTorus(float innerRadius, float outerRadius, int resolution, vec4 color);
};
