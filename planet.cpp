#include "planet.h"

// implement constructor of Planet class
Planet::Planet(vec3 position, float radius, vec4 color, int resolution, GLuint program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
    this->radius = radius;
	this->color = color;
	this->resolution = resolution;

	localVerticePositions = std::vector<vec4>();
	verticeColors = std::vector<vec4>();
	triangles = std::vector<int>();

	// generate uv sphere vertice positions and colors
	for (int i = 0; i < resolution; i++) {
		float alpha = (1.0f * i / (resolution-1));

		// start from the bottom
		vec4 vertice = vec4(0.0f, 0.0f, (-1.0f) * radius, 1.0f);

		mat4 rotationMatrix = RotateX(180.0f * alpha);
		vertice = rotationMatrix * vertice;
		for (int j = 0; j < resolution; j++) {
			float beta = (1.0f * j / resolution);
			vec4 localVertice = vertice;
			rotationMatrix = RotateZ(360.0f * beta);
			localVertice = rotationMatrix * localVertice;
			localVerticePositions.push_back(localVertice);

			// move vertice to its global position
			mat4 translationMatrix = Translate(position.x, position.y, position.z);
			vec4 globalVertice = translationMatrix * localVertice;
			globalVerticePositions.push_back(globalVertice);

			// place vertice color
			verticeColors.push_back(color);
		}
	}

	// generate triangle formation
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

	setupBuffers(program);
}