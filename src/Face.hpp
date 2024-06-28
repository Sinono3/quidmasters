#pragma once
#include <SFML/Graphics.hpp>

struct Face {
	sf::Vector2f position;
	// 1.0 = happy, -1.0 = sad
	float frown = 1.0;

	Face(sf::Vector2f);
	void draw(sf::RenderTarget &window);
};
