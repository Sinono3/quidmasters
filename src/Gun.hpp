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

struct Gun {
	/// Seconds of firePeriod
	Var* firePeriod;
	/// HP to remove
	Var* damage;
	/// Fraction of velocity transmitted to hit entity
	Var* knockback;
	/// Velocity
	Var* bulletSpeed;
};
