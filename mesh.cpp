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

void Mesh::setScale(vec3 scale) {
	this->scale = scale;
	
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

void Mesh::setParentScale(vec3 scale) {
	this->parentScale = scale;
	
	recalculateModelMatrix();
}

void Mesh::recalculateModelMatrix() {
	// calculate global scale
	vec3 globalScale = vec3(parentScale.x * scale.x, parentScale.y * scale.y, parentScale.z * scale.z);

	// apply parent's rotation to the local position
	mat4 parentRotationMatrix = RotateZ(parentRotation.z) * RotateY(parentRotation.y) * RotateX(parentRotation.x);
	vec4 rotatedPosition = parentRotationMatrix * position;

	// calculate global position
	vec4 globalPosition = parentPosition + rotatedPosition;
	globalPosition.w = 1.0;

	// calculate global rotation
	vec4 globalRotation = parentRotation + rotation;

	// construct the model matrix
	modelMatrix = Translate(globalPosition) * RotateZ(globalRotation.z) * RotateY(globalRotation.y) * RotateX(globalRotation.x) * Scale(globalScale);
}

// static functions
Mesh* Mesh::drawSphere(float radius, int resolution, vec4 color) {
	Mesh* mesh = new Mesh();

	// generate uv sphere vertice positions and colors
	for (int i = 0; i < resolution; i++) {
		float alpha = (1.0f * i / (resolution - 1));

		// start from the bottom
		vec4 vertice = vec4(0.0f, 0.0f, (-1.0f) * radius, 1.0f);

		mat4 rotationMatrix = RotateX(180.0f * alpha);
		vertice = rotationMatrix * vertice;
		for (int j = 0; j < resolution; j++) {
			float beta = (1.0f * j / resolution);
			vec4 localVertice = vertice;
			rotationMatrix = RotateZ(360.0f * beta);
			localVertice = rotationMatrix * localVertice;

			mesh->addVertice(localVertice, color);
		}
	}

	// generate triangle formation
	std::vector<int> triangles;
	for (int i = 0; i < resolution - 1; i++) {
		for (int j = 0; j < resolution - 1; j++) {
			triangles.push_back(i * resolution + j);
			triangles.push_back(i * resolution + j + 1);
			triangles.push_back((i + 1) * resolution + j);
			triangles.push_back(i * resolution + j + 1);
			triangles.push_back((i + 1) * resolution + j + 1);
			triangles.push_back((i + 1) * resolution + j);
		}
	}
	mesh->setTriangleArray(triangles);

	return mesh;
}

Mesh* Mesh::drawTorus(float innerRadius, float outerRadius, int resolution, vec4 color) {
	Mesh* mesh = new Mesh();

	vec4 midpoint = vec4((outerRadius - innerRadius) / 2, 0, 0, 1);
	for (int i = 0; i < resolution; i++) {
		float alpha = (1.0f * i / (resolution - 1));
		for (int j = 0; j < resolution; j++) {
			float beta = (1.0f * j / resolution);
			mat4 rotationMatrix = RotateY(360.0f * beta);
			vec4 vertice = rotationMatrix * midpoint;
			mat4 translationMatrix = Translate(innerRadius, 0, 0);
			vertice = translationMatrix * vertice;
			rotationMatrix = RotateZ(360.0f * alpha);
			vertice = rotationMatrix * vertice;

			mesh->addVertice(vertice, color);
		}
	}

	// generate triangle formation
	std::vector<int> triangles;
	for (int i = 0; i < resolution - 1; i++) {
		for (int j = 0; j < resolution - 1; j++) {
			triangles.push_back(i * resolution + j);
			triangles.push_back(i * resolution + j + 1);
			triangles.push_back((i + 1) * resolution + j);
			triangles.push_back(i * resolution + j + 1);
			triangles.push_back((i + 1) * resolution + j + 1);
			triangles.push_back((i + 1) * resolution + j);
		}
	}
	mesh->setTriangleArray(triangles);

	return mesh;
}

Mesh* Mesh::drawTetrahedron(float base, float length, vec4 color) {
	Mesh* mesh = new Mesh();

	float mid = base / 2;
	vec4 top = vec4(0, 0, length, 1);
	vec4 bottom1 = vec4(-mid, -mid, 0, 1);
	vec4 bottom2 = vec4(mid, -mid, 0, 1);
	vec4 bottom3 = vec4(mid, mid, 0, 1);
	vec4 bottom4 = vec4(-mid, mid, 0, 1);
	mesh->addVertice(bottom1, color);
	mesh->addVertice(bottom2, color);
	mesh->addVertice(bottom3, color);
	mesh->addVertice(bottom4, color);
	mesh->addVertice(top, color);

	// generate triangle formation
	std::vector<int> triangles;
	triangles.push_back(0);
	triangles.push_back(4);
	triangles.push_back(1);
	triangles.push_back(1);
	triangles.push_back(4);
	triangles.push_back(2);
	triangles.push_back(2);
	triangles.push_back(4);
	triangles.push_back(3);
	triangles.push_back(3);
	triangles.push_back(4);
	triangles.push_back(0);
	mesh->setTriangleArray(triangles);

	return mesh;
}