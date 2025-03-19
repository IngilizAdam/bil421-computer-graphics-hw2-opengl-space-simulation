#pragma once

#include "Angel.h"
#include <vector>

class Mesh {
	std::vector<vec4> localVerticePositions;
	std::vector<vec4> verticeColors;
	std::vector<int> triangles;

	vec4 position;
	vec4 rotation;

	vec4 parentPosition;
	vec4 parentRotation;

	mat4 modelMatrix;

	void recalculateModelMatrix();

public:
	Mesh() {
		position = vec4(0.0, 0.0, 0.0, 1.0);
		rotation = vec4(0.0, 0.0, 0.0, 0.0);
		modelMatrix = identity();
	}

	void addVertice(vec4 vertice, vec4 color);
	void setTriangleArray(std::vector<int> triangles);

	void setPosition(vec3 position);
	void setPosition(vec4 position);
	void setRotation(vec3 rotation);

	void setParentPosition(vec4 position);
	void setParentRotation(vec4 rotation);

	mat4 getModelMatrix() { return modelMatrix; }
	vec4* getLocalVerticePositions() { return localVerticePositions.data(); }
	vec4* getVerticeColors() { return verticeColors.data(); }
	int* getTriangles() { return triangles.data(); }
	unsigned int getVerticeCount() { return localVerticePositions.size(); }
	unsigned int getTrianglesCount() { return triangles.size(); }
	unsigned int getVerticePositionsSize() { return sizeof(vec4) * localVerticePositions.size(); }
	unsigned int getVerticeColorsSize() { return sizeof(vec4) * verticeColors.size(); }
};