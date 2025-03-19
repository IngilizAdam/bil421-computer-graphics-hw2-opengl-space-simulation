#include "mesh.h"

void Mesh::addVertice(vec4 vertice, vec4 color) {
	localVerticePositions.push_back(vertice);
	verticeColors.push_back(color);
}

void Mesh::setTriangleArray(std::vector<int> triangles) {
	this->triangles = triangles;
}

void Mesh::setPosition(vec3 position) {
	this->position = vec4(position.x, position.y, position.z, 1.0);

	recalculateModelMatrix();
}

void Mesh::setPosition(vec4 position) {
	setPosition(vec3(position.x, position.y, position.z));
}

void Mesh::setRotation(vec3 rotation) {
	this->rotation = vec4(rotation.x, rotation.y, rotation.z, 0.0);

	recalculateModelMatrix();
}

void Mesh::setParentPosition(vec4 position) {
	this->parentPosition = position;

	recalculateModelMatrix();
}

void Mesh::setParentRotation(vec4 rotation) {
	this->parentRotation = rotation;

	recalculateModelMatrix();
}

void Mesh::recalculateModelMatrix() {
	vec4 globalPosition = parentPosition + position;
	vec4 globalRotation = parentRotation + rotation;

	modelMatrix = Translate(globalPosition) * RotateX(globalRotation.x) * RotateY(globalRotation.y) * RotateZ(globalRotation.z);
}
