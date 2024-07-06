#pragma once
#include <random>
#include <variant>
#include "Assets.hpp"

// Uniform random variable
class UniVar  {
  public:
	float from;
	float to;

	constexpr UniVar(float from, float to) : from(from), to(to) {}

	float get(std::default_random_engine &rng) const {
		return (std::uniform_real_distribution<float>(from, to))(rng);
	}
};

struct Gun {
	enum class Type {
		Handgun,
		Shotgun,
		MachineGun,
		Homing
	};

	// Determines the sound it will make and other stuff
	Type type;
	// Determines which icon it will be displayed with
	Assets::Textures::Icon icon;
	/// Seconds of firePeriod
	UniVar firePeriod;
	/// HP to remove
	UniVar damage;
	/// Fraction of velocity transmitted to hit entity
	UniVar knockback;
	/// Velocity
	UniVar bulletSpeed;
	/// Accuracy = what random angle is added when shooting the bullet
	UniVar accuracy;
	/// Projectiles per shot
	int bulletsPerFire;
};
