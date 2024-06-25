#include "Model.hpp"
#include "Shader.hpp"
#include "draw.hpp"
#include <SFML/Graphics.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glad/glad.h>
#include <cmath>
#include <iostream>
#include <optional>

const char map[][10] = {
	{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
	{'W', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', 'P', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
};

int main() {
	sf::ContextSettings settings;
	settings.majorVersion = 4;
	settings.minorVersion = 0;

	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "A lonely dungeon",
							sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	gladLoadGL();
	glEnable(GL_DEPTH);

	// Compile shader
	const char *vertex = R"glsl(
		#version 400

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
	const char *fragment = R"glsl(
		#version 400

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
	Shader shader(vertex, fragment);

	float vertices[] = {
		0.5f,  0.5f,  0.0f, // top right
		0.5f,  -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f,  0.0f	// top left
	};
	unsigned int indices[] = {
		// note that we start from 0!
		0, 1, 3, // first Triangle
		1, 2, 3	 // second Triangle
	};
	Model model(vertices, indices);

	sf::Vector2f playerSpawnPos;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 10; j++) {
			if (map[i][j] == 'P') {
				playerSpawnPos = sf::Vector2f(i, j);
			}
		}
	}

	sf::Vector2i mousePos;
	sf::Vector2i mouseDelta;
	sf::Vector2f playerPos;
	float pitch, yaw;

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}


		// UPDATE --------------------------------------
		// Mouse look
		sf::Vector2i screenSize(window.getSize().x, window.getSize().y);
		sf::Vector2i screenCenter = window.getPosition() + (screenSize / 2);
		sf::Vector2i newMousePos = sf::Mouse::getPosition();
		mouseDelta = newMousePos - screenCenter;
		mousePos = newMousePos;
	    sf::Mouse::setPosition(screenCenter);

		const float MOUSE_SENSITIVITY = 0.001f;
		yaw = yaw - mouseDelta.x * MOUSE_SENSITIVITY;
		pitch = glm::clamp(pitch - mouseDelta.y * MOUSE_SENSITIVITY, -1.5f, 1.5f);

		// Walk 
		sf::Vector2f wasd;
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		    wasd.y = -1.0;
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		    wasd.y = 1.0;
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		    wasd.x = -1.0;
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		    wasd.x = 1.0;
		const float WALK_SPEED = 0.02f;
		// sf::Vector2f walkVector = sf::Vector2f(
		// 	wasd.x,
		// 	wasd.y
		// );
		wasd.y *= -1;
		sf::Vector2f walkVector = sf::Vector2f(
			wasd.x * cos(yaw) - wasd.y * sin(yaw),
			-(wasd.x * sin(yaw) + wasd.y * cos(yaw))
		);
		playerPos += WALK_SPEED * walkVector;

		// DRAW -------------------------------------------

		glClearColor(0.01f, 0.01f, 0.02f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Uniforms uniforms;

		auto time = clock.getElapsedTime().asSeconds();
		auto projection = glm::perspective(3.14f / 2.0f, (float)screenSize.x / (float)screenSize.y, 0.1f, 1000.0f);
		// auto view =
		// 	glm::lookAt(glm::vec3(2.0, 2.0, 2.0), glm::vec3(0.0, 0.0, 0.0),
		// 				glm::vec3(0.0, 1.0, 0.0));

		// camera transform
		glm::mat4 cameraTransform = glm::identity<glm::mat4>();
		// Translation
		cameraTransform = glm::translate(cameraTransform, glm::vec3(playerPos.x, 0.0, playerPos.y));
		// Yaw
		cameraTransform = glm::rotate(cameraTransform, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		// Pitch
		cameraTransform = glm::rotate(cameraTransform, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		// View = inverse of camera "isometry"
		glm::mat4 view = glm::inverse(cameraTransform);
		// auto view = cameraTransform;

		glm::mat4 modelMat = glm::identity<glm::mat4>();
		// modelMat = glm::rotate(modelMat, time, glm::vec3(0.0f, 1.0f, 0.0f));

		uniforms.insert({"time", Uniform1f{time}});
		uniforms.insert({"projection", Uniform4x4f{projection}});
		uniforms.insert({"view", Uniform4x4f{view}});
		uniforms.insert({"model", Uniform4x4f{modelMat}});
		draw(shader, model, uniforms);
		window.display();
	}
}
