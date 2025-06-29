#pragma once
#include "math/Vector2.hpp"
#include <SFML/Graphics/Color.hpp>
#include <random>

struct Enemy {
	Vector2f pos = {5.0f, 5.0f};
	Vector2f vel;
	float maxSpeed, acceleration;
	float health, maxHealth;
	float radius; // Radius of enemy circle
	float damagePerSecond;
	sf::Color color;

	// how many coins will this enemy get you
	int calculateNetWorth() const {
		return (int)(maxHealth * 0.6f);
	}

	int calculateRandomQuidDrop(std::default_random_engine& rng) const {
		int netWorth = calculateNetWorth();
		int minQuidDrop = netWorth / 2;
		int maxQuidDrop = netWorth;
		return std::uniform_int_distribution(minQuidDrop, maxQuidDrop)(rng);
	}
};

struct EnemyClass {
	float maxSpeed = 48.0f, acceleration = 30.0f;
	float maxHealth = 10.0f;
	float radius = 2.0f;
	float damagePerSecond = 0.1f;
	std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> color;

	Enemy produce() const {
		return Enemy {
			.maxSpeed = maxSpeed,
			.acceleration = acceleration,
			.health = maxHealth,
			.maxHealth = maxHealth,
			.radius = radius,
			.damagePerSecond = damagePerSecond,
			.color = sf::Color(
			                   std::get<0>(color), 
			                   std::get<1>(color), 
			                   std::get<2>(color),
			                   std::get<3>(color)),
		};
	}
};

