#include "math/Vector2.hpp"

struct QuidDrop {
	Vector2f pos;
	int quid;

	float getRadius() const {
		return quid / 10.0f;
	}
};
