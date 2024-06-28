#pragma once
#include "math/Vector2.hpp"
#include <SFML/Graphics/Color.hpp>

struct Enemy {
	Vector2f pos = {5.0f, 5.0f};
	Vector2f vel;
	float maxSpeed, acceleration;
	float health, maxHealth;
	float radius; // Radius of enemy circle
	sf::Color color;
};

struct EnemyClass {
	float maxSpeed = 48.0f, acceleration = 30.0f;
	float maxHealth = 10.0f;
	float radius = 2.0f;
	sf::Color color;

	Enemy produce() {
		return Enemy {
			.maxSpeed = maxSpeed,
			.acceleration = acceleration,
			.health = maxHealth,
			.maxHealth = maxHealth,
			.radius = radius,
			.color = color,
		};
	}
};
