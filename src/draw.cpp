#include "draw.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
 
// helper type for the visitor #4
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void draw(const Shader &shader, const Model &model, Uniforms uniforms) {
	shader.bind();

	// Load uniforms
    for ( const auto &kv : uniforms ) {
    	auto name = kv.first;
    	auto value = kv.second;

		GLuint location = shader.getUniformLocation(name);

		std::visit(overloaded {
		  [&](Uniform1f u) { glUniform1f(location, u.x); },
		  [&](Uniform4x4f u) { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(u.mat)); },
		}, value);
    }

	model.draw();
	shader.unbind();
}
