#pragma once
#include "Model.hpp"
#include "Shader.hpp"
#include <map>
#include <glm/glm.hpp>

struct Uniform1f {
	float x;
};
struct Uniform4x4f {
	glm::mat4 mat;
};
using Uniform = std::variant<Uniform1f, Uniform4x4f>;
using Uniforms = std::map<std::string, Uniform>;

void draw(const Shader &shader, const Model &model, Uniforms uniforms);
