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

constexpr const char *vertex = R"glsl(
	#version 330 core

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;

	layout(location = 0) in vec3 position;
	out vec3 viewPosition;

	void main() {
		viewPosition = (view * model * vec4(position, 1.0)).xyz;
		gl_Position = projection * view * model * vec4(position, 1.0);
	}
)glsl";
constexpr const char *fragment = R"glsl(
	#version 330 core

	uniform float time;

	in vec3 viewPosition;
	out vec4 color;

	float sin01(float x) {
		return (sin(x) + 1.0f) / 2.0f;
	}

	void main() {
		// Distance to camera
		vec3 cameraToFragment = viewPosition;
		float lightingLevel = 1.0f / dot(cameraToFragment, cameraToFragment);
		float noisedLightingLevel = 
			  (0.2f * lightingLevel * sin01(time * 1.0f + 5.0f))
			+ (0.2f * lightingLevel * sin01(time * 3.0f + 4.0f))
			+ (0.2f * lightingLevel * sin01(time * 5.0f + 3.0f))
			+ (0.2f * lightingLevel * sin01(time * 7.0f + 2.0f))
			+ (0.2f * lightingLevel * sin01(time * 9.0f + 1.0f));
		vec4 darkness = vec4(0.01f, 0.01f, 0.02f, 1.0f);
		vec4 trueColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		color = mix(darkness, trueColor, noisedLightingLevel);
	}
)glsl";
