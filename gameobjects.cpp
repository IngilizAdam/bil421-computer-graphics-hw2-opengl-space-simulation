#include "gameobjects.h"

void GameObject::setupBuffers(GLuint program) {
    // Create a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &buffer);

    // Create and initialize a buffer object
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, getVerticePositionsSize() + getVerticeColorsSize(),
        NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, getVerticePositionsSize(), globalVerticePositions.data());
    glBufferSubData(GL_ARRAY_BUFFER, getVerticePositionsSize(), getVerticeColorsSize(), verticeColors.data());

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
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, triangles.data());
}