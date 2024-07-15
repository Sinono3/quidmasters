#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "math/Vector2.hpp"

struct Player {
	// defined in `definitions.cpp`
	static const float RADIUS;
	static const float RADIUS_SQ;

	Vector2f pos;
	Vector2f vel;
    float angle = 0.0f;
    float health = 10.0f, maxHealth = 10.0f;
    float nourishment = 10.0f, maxNourishment = 10.0f;
    float sanity = 10.0f, maxSanity = 10.0f;
    int coins = 0;

    Vector2f getForward() const {
    	return transformVector(Vector2f(1.0f, 0.0f));
	}

    Vector2f transformVector(const Vector2f& vector) const {
    	return vector.rotate(angle);
    }

	Vector2f inverseTransformVector(const Vector2f& vector) const {
    	return vector.rotate(-angle);
	}
};

