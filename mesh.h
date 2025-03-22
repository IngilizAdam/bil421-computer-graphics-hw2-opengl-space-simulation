#pragma once

#include "Angel.h"
#include <vector>

class Mesh {
	std::vector<vec4> localVerticePositions;
	std::vector<vec4> verticeColors;
	std::vector<int> triangles;
	std::vector<vec3> verticeNormals;

	vec4 position;
	vec4 rotation;
	vec3 scale;

	vec4 parentPosition;
	vec4 parentRotation;
	vec3 parentScale;

	mat4 modelMatrix;

	void recalculateModelMatrix();
	void recalculateNormals();

public:
	Mesh() {
		position = vec4(0.0, 0.0, 0.0, 1.0);
		rotation = vec4(0.0, 0.0, 0.0, 0.0);
		scale = vec3(1.0, 1.0, 1.0);
		modelMatrix = identity();
	}

	void addVertice(vec4 vertice, vec4 color);
	void setTriangleArray(std::vector<int> triangles);

	void setPosition(vec3 position);
	void setPosition(vec4 position);
	void setRotation(vec3 rotation);
	void setScale(vec3 scale);

	void setParentPosition(vec4 position);
	void setParentRotation(vec4 rotation);
	void setParentScale(vec3 scale);

	mat4 getModelMatrix() { return modelMatrix; }
	vec4* getLocalVerticePositions() { return localVerticePositions.data(); }
	vec4* getVerticeColors() { return verticeColors.data(); }
	vec3* getVerticeNormals() { return verticeNormals.data(); }
	int* getTriangles() { return triangles.data(); }
	unsigned int getVerticeCount() { return localVerticePositions.size(); }
	unsigned int getTrianglesCount() { return triangles.size(); }
	unsigned int getVerticePositionsSize() { return sizeof(vec4) * localVerticePositions.size(); }
	unsigned int getVerticeColorsSize() { return sizeof(vec4) * verticeColors.size(); }
	unsigned int getVerticeNormalsSize() { return sizeof(vec3) * verticeNormals.size(); }

	// static functions
	static Mesh* drawPlane(float width, float height, vec4 color);
	static Mesh* drawSphere(float radius, int resolution, vec4 color);
	static Mesh* drawTorus(float innerRadius, float outerRadius, int resolution, vec4 color);
	static Mesh* drawTetrahedron(float base, float length, vec4 color);
};