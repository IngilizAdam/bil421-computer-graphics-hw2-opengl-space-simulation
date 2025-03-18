#pragma once

#include "Angel.h"
#include <vector>

typedef struct {
	std::vector<vec4> globalVerticePositions;
	std::vector<vec4> localVerticePositions;
	std::vector<vec4> verticeColors;
	std::vector<int> triangles;
} Mesh;

class GameObject {

protected:
	vec4 position;
	vec4 color;

	Mesh mesh;

	GLuint vao;
	GLuint buffer;
	GLuint program;

public:
	GameObject() {}

	GameObject(GLuint program) {
		this->program = program;
		setupBuffers();
	}

	void setupBuffers();
	void updateBuffers();

	unsigned int getVerticePositionsSize() { return sizeof(vec4) * mesh.globalVerticePositions.size(); }
	unsigned int getVerticeColorsSize() { return sizeof(vec4) * mesh.verticeColors.size(); }
	unsigned int getTrianglesSize() { return sizeof(int) * mesh.triangles.size(); }
	unsigned int getTrianglesCount() { return mesh.triangles.size(); }

	static Mesh drawSphere(float radius, int resolution, vec4 color, vec3 position);
};
