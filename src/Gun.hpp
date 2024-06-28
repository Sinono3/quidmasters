#pragma once
#include <random>
#include <variant>

class Var {
  public:
	Var() {}
	virtual float get(std::default_random_engine &rng) = 0;
};

// Normally distributed random variable
class NDVar : public Var {
  public:
	float mean;
	float stddev;

	NDVar(float mean, float stddev) : mean(mean), stddev(stddev) {}

	float get(std::default_random_engine &rng) override {
		return (std::normal_distribution<float>(mean, stddev))(rng);
	}
};

// Uniform random variable
class UniVar : public Var {
  public:
	float from;
	float to;

	UniVar(float from, float to) : from(from), to(to) {}

	float get(std::default_random_engine &rng) override {
		return (std::uniform_real_distribution<float>(from, to))(rng);
	}
};

class ConstVar : public Var {
  public:
	float value;

	ConstVar(float value) : value(value) {}
	float get(std::default_random_engine &rng) override { return value; }
};

enum GunType {
	Handgun,
	Shotgun,
	MachineGun,
};

struct Gun {
	// Determines the sound it will make and other stuff
	GunType type;
	/// Seconds of firePeriod
	Var *firePeriod;
	/// HP to remove
	Var *damage;
	/// Fraction of velocity transmitted to hit entity
	Var *knockback;
	/// Velocity
	Var *bulletSpeed;
	/// Accuracy = what random angle is added when shooting the bullet
	Var *accuracy;
	/// Projectiles per shot
	int bulletsPerFire;
};

const Gun GUN_SHODDY_PISTOL = {
	.type = Handgun,
	.firePeriod = new UniVar(0.7f, 1.6f),
	.damage = new UniVar(3.9f, 4.1f),
	.knockback = new UniVar(0.0f, 0.2f),
	.bulletSpeed = new UniVar(50.0f, 100.0f),
	.accuracy = new UniVar(-0.2f, 0.2f),
	.bulletsPerFire = 1,
};
const Gun GUN_OKAYISH_PISTOL = {
	.type = Handgun,
	.firePeriod = new UniVar(0.2f, 0.7f),
	.damage = new UniVar(3.9f, 4.1f),
	.knockback = new UniVar(0.0f, 0.2f),
	.bulletSpeed = new UniVar(80.0f, 100.0f),
	.accuracy = new UniVar(-0.02f, 0.02f),
	.bulletsPerFire = 1,
};
const Gun GUN_RIGHTSIDE_SHOTGUN = {
	.type = Shotgun,
	.firePeriod = new UniVar(0.7f, 1.0f),
	.damage = new UniVar(4.9f, 6.1f),
	.knockback = new UniVar(0.0f, 0.4f),
	.bulletSpeed = new UniVar(200.0f, 300.0f),
	.accuracy = new UniVar(-0.02f, 1.0f),
	.bulletsPerFire = 5,
};
const Gun GUN_MACHINE_BOY = {
	.type = MachineGun,
	.firePeriod = new UniVar(0.08f, 0.1f),
	.damage = new UniVar(1.0f, 1.8f),
	.knockback = new UniVar(0.0f, 0.15f),
	.bulletSpeed = new UniVar(100.0f, 200.0f),
	.accuracy = new UniVar(-0.02f, 0.02f),
	.bulletsPerFire = 2,
};
const Gun GUN_EXPLO = {
	.type = Shotgun,
	.firePeriod = new UniVar(0.08f, 0.8f),
	// .firePeriod = new UniVar(8.08f, 12.1f),
	.damage = new UniVar(5.0f, 4.8f),
	.knockback = new UniVar(0.0f, 0.4f),
	.bulletSpeed = new UniVar(100.0f, 200.0f),
	.accuracy = new UniVar(-3.14f, 3.14f),
	.bulletsPerFire = 50,
};
