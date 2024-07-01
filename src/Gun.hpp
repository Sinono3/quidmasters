#pragma once
#include <random>
#include <variant>

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
	};

	// Determines the sound it will make and other stuff
	Type type;
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

constexpr Gun GUN_SHODDY_PISTOL = {
	.type = Gun::Type::Handgun,
	.firePeriod = UniVar(0.7f, 1.6f),
	.damage = UniVar(3.9f, 4.1f),
	.knockback = UniVar(0.0f, 0.2f),
	.bulletSpeed = UniVar(50.0f, 100.0f),
	.accuracy = UniVar(-0.2f, 0.2f),
	.bulletsPerFire = 1,
};
constexpr Gun GUN_OKAYISH_PISTOL = {
	.type = Gun::Type::Handgun,
	.firePeriod = UniVar(0.2f, 0.7f),
	.damage = UniVar(3.9f, 4.1f),
	.knockback = UniVar(0.0f, 0.2f),
	.bulletSpeed = UniVar(80.0f, 100.0f),
	.accuracy = UniVar(-0.02f, 0.02f),
	.bulletsPerFire = 1,
};
constexpr Gun GUN_RIGHTSIDE_SHOTGUN = {
	.type = Gun::Type::Shotgun,
	.firePeriod = UniVar(0.7f, 1.0f),
	.damage = UniVar(4.9f, 6.1f),
	.knockback = UniVar(0.0f, 0.4f),
	.bulletSpeed = UniVar(200.0f, 300.0f),
	.accuracy = UniVar(-0.02f, 1.0f),
	.bulletsPerFire = 5,
};
constexpr Gun GUN_MACHINE_BOY = {
	.type = Gun::Type::MachineGun,
	.firePeriod = UniVar(0.08f, 0.1f),
	.damage = UniVar(1.0f, 1.8f),
	.knockback = UniVar(0.0f, 0.15f),
	.bulletSpeed = UniVar(100.0f, 200.0f),
	.accuracy = UniVar(-0.02f, 0.02f),
	.bulletsPerFire = 2,
};
constexpr Gun GUN_EXPLO = {
	.type = Gun::Type::Shotgun,
	.firePeriod = UniVar(0.08f, 0.8f),
	// .firePeriod = UniVar(8.08f, 12.1f),
	.damage = UniVar(5.0f, 4.8f),
	.knockback = UniVar(0.0f, 0.4f),
	.bulletSpeed = UniVar(100.0f, 200.0f),
	.accuracy = UniVar(-3.14f, 3.14f),
	.bulletsPerFire = 200,
};
