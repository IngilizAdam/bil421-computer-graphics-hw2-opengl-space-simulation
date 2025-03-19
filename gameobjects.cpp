#include "gameobjects.h"

void GameObject::setupBuffers() {
    // Create a vertex array object for each mesh
	for (int i = 0; i < meshs.size(); i++) {
		Mesh mesh = meshs[i];
		GLuint vao;
		GLuint buffer;
		GLuint program = this->program;
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
    
		glGenBuffers(1, &buffer);

		// Create and initialize a buffer object
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, mesh.getVerticePositionsSize() + mesh.getVerticeColorsSize(),
			NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.getVerticePositionsSize(), mesh.getLocalVerticePositions());
		glBufferSubData(GL_ARRAY_BUFFER, mesh.getVerticePositionsSize(), mesh.getVerticeColorsSize(), mesh.getVerticeColors());

		// set up vertex arrays
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

		GLuint vColor = glGetAttribLocation(program, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(mesh.getVerticePositionsSize()));

		this->vao.push_back(vao);
		this->buffer.push_back(buffer);
	}
}

void GameObject::updateBuffers() {
	for (int i = 0; i < meshs.size(); i++) {
		Mesh mesh = meshs[i];
		GLuint vao = this->vao[i];
		GLuint buffer = this->buffer[i];

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		// send model matrix to shader
		glUniformMatrix4fv(model, 1, GL_TRUE, mesh.getModelMatrix());

		glDrawElements(GL_TRIANGLES, mesh.getTrianglesCount(), GL_UNSIGNED_INT, mesh.getTriangles());
	}
}

void GameObject::updatePhysics() {
	setPosition(position + velocity);
}

void GameObject::setPosition(vec4 position) {
	this->position = position;
	
	for (int i = 0; i < meshs.size(); i++) {
		meshs[i].setParentPosition(position);
	}
}

void GameObject::setRotation(vec4 rotation) {
	this->rotation = rotation;
	
	for (int i = 0; i < meshs.size(); i++) {
		meshs[i].setParentRotation(rotation);
	}
}

void GameObject::setVelocity(vec4 velocity) {
	this->velocity = velocity;
}

Mesh GameObject::drawSphere(float radius, int resolution, vec4 color) {
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

			mesh.addVertice(localVertice, color);
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
	mesh.setTriangleArray(triangles);

	return mesh;
}

Mesh GameObject::drawTorus(float innerRadius, float outerRadius, int resolution, vec4 color) {
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

			mesh.addVertice(vertice, color);
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
	mesh.setTriangleArray(triangles);

	return mesh;
}

Mesh GameObject::drawTetrahedron(float base, float length, vec4 color) {
	Mesh mesh;

	float mid = base / 2;
	vec4 top = vec4(0, 0, length, 1);
	vec4 bottom1 = vec4(-mid, -mid, 0, 1);
	vec4 bottom2 = vec4(mid, -mid, 0, 1);
	vec4 bottom3 = vec4(mid, mid, 0, 1);
	vec4 bottom4 = vec4(-mid, mid, 0, 1);
	mesh.addVertice(bottom1, color);
	mesh.addVertice(bottom2, color);
	mesh.addVertice(bottom3, color);
	mesh.addVertice(bottom4, color);
	mesh.addVertice(top, color);

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
	mesh.setTriangleArray(triangles);

	return mesh;
}