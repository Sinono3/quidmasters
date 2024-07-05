#pragma once
#include "Enemy.hpp"

constexpr float TILE_SIZE = 10.0f;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr float RADIUS = 1.0f;

constexpr float WAVE_TIME = 30.0f;
constexpr float BREAK_TIME = 10.0f;
// All enemies must have spawned in this time
// constexpr float ENEMY_SPAWN_TIME = 10.0f;

constexpr int ENEMY_CLASS_COUNT = 3;
const EnemyClass ENEMY_CLASSES[ENEMY_CLASS_COUNT]{
	// Standard zombie
	{
		.maxSpeed = 10.0f, .acceleration = 180.0f,
		.maxHealth = 10.0f,
		.radius = 1.0f,
		.damagePerSecond = 0.1f,
		.color = sf::Color(0, 200, 0, 255),
	},
	// big zomba
	{
		.maxSpeed = 5.0f, .acceleration = 180.0f,
		.maxHealth = 40.0f,
		.radius = 2.0f,
		.damagePerSecond = 0.1f,
		.color = sf::Color(100, 0, 100, 255),
	},
	// quick zomb
	{
		.maxSpeed = 80.0f, .acceleration = 180.0f,
		.maxHealth = 3.0f,
		.radius = 0.5f,
		.damagePerSecond = 0.1f,
		.color = sf::Color(255, 0, 255, 255),
		.kind = Enemy::Kind::Grav
	},
};

using EnemyClassCounter = std::array<int, ENEMY_CLASS_COUNT>;
