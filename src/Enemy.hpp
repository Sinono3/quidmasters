#include "math/Vector2.hpp"

struct Enemy {
	Vector2f pos = {5.0f, 5.0f};
	Vector2f vel;
	float maxSpeed, acceleration;
	float maxHealth;
	float size;
};

struct EnemyClass {
	float maxSpeed = 48.0f, acceleration = 30.0f;
	float maxHealth = 10.0f;
	float size = 2.0f;

	Enemy produce() {
		return Enemy {
			.maxSpeed = maxSpeed,
			.acceleration = acceleration,
			.maxHealth = maxHealth,
			.size = size,
		};
	}
};
