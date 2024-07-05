#include "Gun.hpp"
#include <cmath>

// A gun's specifications and its state
struct GunState {
	// Specs
	Gun specs;
	// How long will the weapon cooldown for.
	float cooldown;
	// How much time has passed since the weapon started cooling down
	// cooldownTime <= cooldown
	float cooldownTimer;

	// Create a default gun state from a gun
	GunState(Gun gun) : specs(gun) {
		cooldown = 1.0f;
		cooldownTimer = 1.0f;
	}

	/// Advance the cooldownTimer value
	void advanceCooldownTimer(float dt) {
		cooldownTimer = std::min(cooldownTimer + dt, cooldown);
	}

	// Reset cooldown timer
	void resetCooldownTimer(float newCooldown) {
		cooldownTimer = 0.0f;
		cooldown = newCooldown;
	}

	bool hasCooledDown() const {
		return cooldownTimer >= cooldown;
	}
};
