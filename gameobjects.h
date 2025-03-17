#pragma once

#include "Angel.h"
#include <vector>

class GameObject {
public:
	vec4 position;
	vec4 color;

	std::vector<vec4> globalVerticePositions;
	std::vector<vec4> localVerticePositions;
	std::vector<vec4> verticeColors;
	std::vector<int> triangles;

	GLuint vao;
	GLuint buffer;

	void setupBuffers(GLuint program);
	void updateBuffers();

	unsigned int getVerticePositionsSize() { return sizeof(vec4) * globalVerticePositions.size(); }
	unsigned int getVerticeColorsSize() { return sizeof(vec4) * verticeColors.size(); }
	unsigned int getTrianglesSize() { return sizeof(int) * triangles.size(); }
	unsigned int getTrianglesCount() { return triangles.size(); }
};
