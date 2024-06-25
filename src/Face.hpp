#pragma once
#include <SFML/Graphics.hpp>

struct Face {
	// 1.0 = happy, -1.0 = sad
	float frown = 1.0;

	void draw(sf::RenderTarget &window);
};
