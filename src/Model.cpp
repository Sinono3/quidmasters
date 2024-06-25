#include "Model.hpp"

Model::Model(std::span<GLfloat> positions, std::span<GLuint> indices) {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &positionsVBO);
	glGenBuffers(1, &indicesEBO);

	glBindVertexArray(vao);

	// Positions
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size_bytes(), positions.data(),
				 GL_STATIC_DRAW);

	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(),
				 GL_STATIC_DRAW);

	// Bind positions buffer to VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  (void *)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	vertexCount = indices.size();
}

Model::~Model() {
	if (vao != 0)
		glDeleteVertexArrays(1, &vao);
	if (indicesEBO != 0)
		glDeleteBuffers(1, &indicesEBO);
	if (positionsVBO != 0)
		glDeleteBuffers(1, &positionsVBO);
}

void Model::bind() const {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesEBO);
}

void Model::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::draw() const {
	bind();
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
	unbind();
}
