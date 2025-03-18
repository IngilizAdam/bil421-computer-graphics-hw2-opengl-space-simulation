#include "gameobjects.h"

void GameObject::setupBuffers() {
    // Create a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &buffer);

    // Create and initialize a buffer object
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, getVerticePositionsSize() + getVerticeColorsSize(),
        NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, getVerticePositionsSize(), mesh.globalVerticePositions.data());
    glBufferSubData(GL_ARRAY_BUFFER, getVerticePositionsSize(), getVerticeColorsSize(), mesh.verticeColors.data());

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(0));

    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(getVerticePositionsSize()));
}

void GameObject::updateBuffers() {
	updateExtra();

    glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glDrawElements(GL_TRIANGLES, mesh.triangles.size(), GL_UNSIGNED_INT, mesh.triangles.data());
}

void GameObject::updateExtra() {
	// for derived classes to implement
}

Mesh GameObject::drawSphere(float radius, int resolution, vec4 color, vec3 position) {
	Mesh mesh;

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
			mesh.localVerticePositions.push_back(localVertice);

			// move vertice to its global position
			mat4 translationMatrix = Translate(position.x, position.y, position.z);
			vec4 globalVertice = translationMatrix * localVertice;
			mesh.globalVerticePositions.push_back(globalVertice);

			// place vertice color
			mesh.verticeColors.push_back(color);
		}
	}

	// generate triangle formation
	for (int i = 0; i < resolution - 1; i++) {
		for (int j = 0; j < resolution - 1; j++) {
			mesh.triangles.push_back(i * resolution + j);
			mesh.triangles.push_back(i * resolution + j + 1);
			mesh.triangles.push_back((i + 1) * resolution + j);
			mesh.triangles.push_back(i * resolution + j + 1);
			mesh.triangles.push_back((i + 1) * resolution + j + 1);
			mesh.triangles.push_back((i + 1) * resolution + j);
		}
	}

	return mesh;
}

Mesh GameObject::drawTorus(float innerRadius, float outerRadius, int resolution, vec4 color, vec3 position) {
	Mesh mesh;

	vec4 midpoint = vec4((outerRadius - innerRadius) / 2, 0, 0, 1);
	for (int i = 0; i < resolution; i++) {
		float alpha = (1.0f * i / (resolution-1));
		for (int j = 0; j < resolution; j++) {
			float beta = (1.0f * j / resolution);
			mat4 rotationMatrix = RotateY(360.0f * beta);
			vec4 vertice = rotationMatrix * midpoint;
			mat4 translationMatrix = Translate(innerRadius, 0, 0);
			vertice = translationMatrix * vertice;
			rotationMatrix = RotateZ(360.0f * alpha);
			vertice = rotationMatrix * vertice;
			mesh.localVerticePositions.push_back(vertice);
			mesh.globalVerticePositions.push_back(Translate(position) * vertice);
			mesh.verticeColors.push_back(color);
		}
	}

	// generate triangle formation
	for (int i = 0; i < resolution - 1; i++) {
		for (int j = 0; j < resolution - 1; j++) {
			mesh.triangles.push_back(i * resolution + j);
			mesh.triangles.push_back(i * resolution + j + 1);
			mesh.triangles.push_back((i + 1) * resolution + j);
			mesh.triangles.push_back(i * resolution + j + 1);
			mesh.triangles.push_back((i + 1) * resolution + j + 1);
			mesh.triangles.push_back((i + 1) * resolution + j);
		}
	}

	return mesh;
}