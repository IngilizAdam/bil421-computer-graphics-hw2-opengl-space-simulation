#include "mesh.h"

void Mesh::addVertice(vec4 vertice, vec4 color) {
	localVerticePositions.push_back(vertice);
	verticeColors.push_back(color);
}

void Mesh::setTriangleArray(std::vector<int> triangles) {
	this->triangles = triangles;
}

void Mesh::setPosition(vec4 position) {
	this->position = position;

	modelMatrix = Translate(position);
}

void Mesh::setRotation(vec4 rotation) {
	this->rotation = rotation;

	mat4 rotationMatrix = RotateX(rotation.x) * RotateY(rotation.y) * RotateZ(rotation.z);
	modelMatrix = rotationMatrix * modelMatrix;
}