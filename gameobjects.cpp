#include "gameobjects.h"

void GameObject::setupBuffers() {
    // Create a vertex array object for each mesh
	for (int i = 0; i < meshs.size(); i++) {
		Mesh* mesh = meshs[i];
		GLuint vao;
		GLuint buffer;
		GLuint program = this->program;
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
    
		glGenBuffers(1, &buffer);

		// Create and initialize a buffer object
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, mesh->getVerticePositionsSize() + mesh->getVerticeColorsSize() + mesh->getVerticeNormalsSize(),
			NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->getVerticePositionsSize(), mesh->getLocalVerticePositions());
		glBufferSubData(GL_ARRAY_BUFFER, mesh->getVerticePositionsSize(), mesh->getVerticeColorsSize(), mesh->getVerticeColors());
		glBufferSubData(GL_ARRAY_BUFFER, mesh->getVerticePositionsSize() + mesh->getVerticeColorsSize(), mesh->getVerticeNormalsSize(), mesh->getVerticeNormals());

		// set up vertex arrays
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

		GLuint vColor = glGetAttribLocation(program, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(mesh->getVerticePositionsSize()));

		GLuint vNormal = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(mesh->getVerticePositionsSize() + mesh->getVerticeColorsSize()));

		this->vao.push_back(vao);
		this->buffer.push_back(buffer);
	}
}

void GameObject::updateBuffers() {
	for (int i = 0; i < meshs.size(); i++) {
		Mesh* mesh = meshs[i];
		GLuint vao = this->vao[i];
		GLuint buffer = this->buffer[i];

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		// send model matrix to shader
		glUniformMatrix4fv(model, 1, GL_TRUE, mesh->getModelMatrix());

		glDrawElements(GL_TRIANGLES, mesh->getTrianglesCount(), GL_UNSIGNED_INT, mesh->getTriangles());
	}
}

void GameObject::updatePhysics() {
	setPosition(position + velocity);
	setRotation(rotation + rotationalVelocity);
}

void GameObject::setPosition(vec4 position) {
	this->position = position;
	
	for (int i = 0; i < meshs.size(); i++) {
		meshs[i]->setParentPosition(position);
	}
}

void GameObject::setRotation(vec4 rotation) {
	this->rotation = rotation;
	
	for (int i = 0; i < meshs.size(); i++) {
		meshs[i]->setParentRotation(rotation);
	}
}

void GameObject::setVelocity(vec4 velocity) {
	this->velocity = velocity;
}

void GameObject::setRotationalVelocity(vec4 rotationalVelocity) {
	this->rotationalVelocity = rotationalVelocity;
}

vec4 GameObject::getDirection() {
	mat4 rotationMatrix = RotateZ(rotation.z) * RotateY(rotation.y) * RotateX(rotation.x);
	vec4 direction = rotationMatrix * vec4(0, 1, 0, 0);

	return direction;
}