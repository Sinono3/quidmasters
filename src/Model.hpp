#pragma once
#include <glad/glad.h>
#include <span>
#include <vector>
#include <optional>

class Model {
  GLuint vao = 0;
  GLuint indicesEBO = 0;
  GLuint positionsVBO = 0;
  GLuint vertexCount = 0;
  
  void bind() const;
  void unbind() const;

  public:
    Model(std::span<GLfloat> positions, std::span<GLuint> indices);
    ~Model();

    void draw() const;
};
