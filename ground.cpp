#include "ground.h"
#include "first.h"

Ground::Ground(vec3 position, int subdivision, float width, float height, vec4 color, GLuint program) : GameObject(program) {
	this->position = vec4(position.x, position.y, position.z, 1.0f);
	this->rotation = vec4(0, 0, 0, 0); // draw on xy plane
	this->color = color;

	vec4 nextTileColor = color;

	for (int i = 0; i < subdivision; i++) {
		for (int j = 0; j < subdivision; j++) {
			Mesh* plane = Mesh::drawPlane(width / subdivision, height / subdivision, nextTileColor);
			plane->setPosition(vec3(1.0f * i * (width / subdivision), 1.0f * j * (height / subdivision), 0));
			plane->setParentPosition(this->position);
			plane->setParentRotation(this->rotation);
			plane->setParentScale(vec3(GENERAL_SCALE, GENERAL_SCALE, GENERAL_SCALE));
			meshs.push_back(plane);

			// update color with its complement
			if (nextTileColor.x == color.x && nextTileColor.y == color.y && nextTileColor.z == color.z) {
				nextTileColor = vec4(1.0 - color.x, 1.0 - color.y, 1.0 - color.z, 1.0);
			}
			else {
				nextTileColor = color;
			}
		}
	}

	setupBuffers();
}