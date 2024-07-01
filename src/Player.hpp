#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>
#include "math/Vector2.hpp"

struct Player {
	Vector2f pos = {2.0f, 2.0f};
    float angle = 0.0f;
    float health = 10.0f, maxHealth = 10.0f;
    float nourishment = 10.0f, maxNourishment = 10.0f;
    float sanity = 10.0f, maxSanity = 10.0f;
    int coins = 0;

    Vector2f getForward() const {
    	return transformVector(Vector2f(1.0f, 0.0f));
	}

    Vector2f transformVector(const Vector2f& vector) const {
    	return Vector2f(
	    	vector.x * cos(angle) - vector.y * sin(angle),
	    	vector.x * sin(angle) + vector.y * cos(angle)
	    );
    }

	Vector2f inverseTransformVector(const Vector2f& vector) const {
		return Vector2f(
			vector.x * cos(angle) + vector.y * sin(angle),
			-vector.x * sin(angle) + vector.y * cos(angle)
		);
	}
};

