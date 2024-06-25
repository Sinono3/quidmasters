#pragma once
#include <glad/glad.h>
#include <span>
#include <string>

struct Shader {
 GLuint programID;

 public:
  Shader(std::string vertexSource, std::string fragmentSource);
  ~Shader();

  void bind() const;
  void unbind() const;
  GLuint getUniformLocation(std::string_view name) const;
};
