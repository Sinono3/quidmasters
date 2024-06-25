#include "Model.hpp"
#include "Shader.hpp"
#include "draw.hpp"
#include "math/Vector2.hpp"
#include "Map.hpp"
#include <SFML/Graphics.hpp>
#include <glm/ext.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glad/glad.h>
#include <cmath>
#include <iostream>
#include <optional>

Map<9ul, 10ul> map{{
	{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
	{'W', ' ', 'W', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', 'P', 'W', 'I', ' ', ' ', ' ', 'I', ' ', 'W'},
	{'W', ' ', 'W', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', ' ', 'W', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', ' ', 'W', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', ' ', 'W', 'W', 'W', 'W', ' ', ' ', ' ', 'W'},
	{'W', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'W'},
	{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
}};

int main() {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Attribute::Core;

	sf::Clock clock;
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "A lonely dungeon",
							sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);

	// Compile shader
	Shader shader(vertex, fragment);
	Model model = buildMapModel<9ul, 10ul>(map);

	Vector2f playerSpawnPos;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 10; j++) {
			if (map[i][j] == 'P') {
				playerSpawnPos = sf::Vector2f(j + 0.5, i + 0.5);
			}
		}
	}

	Vector2i mousePos;
	Vector2i mouseDelta;
	Vector2f playerPos;
	float pitch, yaw;

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::Resized) {
				// glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		// UPDATE --------------------------------------
		// Mouse look
		Vector2i screenSize = Vector2(window.getSize()).to<int>();
		Vector2i screenCenter = Vector2i(window.getPosition()) + (screenSize.to<float>() * 0.5f).to<int>();
		Vector2i newMousePos = sf::Mouse::getPosition();
		mouseDelta = newMousePos - screenCenter;
		mousePos = newMousePos;
	    sf::Mouse::setPosition(screenCenter.toSFML());

		const float MOUSE_SENSITIVITY = 0.001f;
		yaw = yaw - mouseDelta.x * MOUSE_SENSITIVITY;
		pitch = glm::clamp(pitch - mouseDelta.y * MOUSE_SENSITIVITY, -1.5f, 1.5f);

		// Walk 
		sf::Vector2f wasd;
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		    wasd.y = 1.0;
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		    wasd.y = -1.0;
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		    wasd.x = -1.0;
	    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		    wasd.x = 1.0;
		const float WALK_SPEED = 0.02f;

		Vector2f walkVector = sf::Vector2f(
			wasd.x * cos(yaw) - wasd.y * sin(yaw),
			-(wasd.x * sin(yaw) + wasd.y * cos(yaw))
		);
		playerPos = playerPos + walkVector * WALK_SPEED;

		// DRAW -------------------------------------------

		// glEnable(GL_DEPTH_TEST);
		// glClearColor(0.01f, 0.01f, 0.02f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
		// // window.clear(sf::Color(0.01f * 255, 0.01f * 255, 0.02f * 255, 1.0f));

		// Uniforms uniforms;

		// auto time = clock.getElapsedTime().asSeconds();
		// auto projection = glm::perspective(3.14f / 2.0f, (float)screenSize.x / (float)screenSize.y, 0.1f, 10.0f);

		// // camera transform
		// glm::mat4 cameraTransform = glm::identity<glm::mat4>();
		// // Translation
		// cameraTransform = glm::translate(cameraTransform, glm::vec3(playerPos.x, 0.5, playerPos.y));
		// // Yaw
		// cameraTransform = glm::rotate(cameraTransform, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		// // Pitch
		// cameraTransform = glm::rotate(cameraTransform, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
		// // View = inverse of camera "isometry"
		// glm::mat4 view = glm::inverse(cameraTransform);
		// glm::mat4 modelMat = glm::identity<glm::mat4>();
		// // modelMat = glm::rotate(modelMat, time, glm::vec3(0.0f, 1.0f, 0.0f));

		// uniforms.insert({"time", Uniform1f{time}});
		// uniforms.insert({"projection", Uniform4x4f{projection}});
		// uniforms.insert({"view", Uniform4x4f{view}});
		// uniforms.insert({"model", Uniform4x4f{modelMat}});
		// draw(shader, model, uniforms);

		window.pushGLStates();
		window.resetGLStates();
	
		// Disable depth testing for 2D rendering
		glDisable(GL_DEPTH_TEST);
		
		sf::RectangleShape shape(sf::Vector2f(100.0f, 100.0f));
		shape.setPosition(400.0, 300.0);
		shape.setFillColor(sf::Color::White);
		window.draw(shape);
		window.popGLStates();

		window.display();
	}
}
