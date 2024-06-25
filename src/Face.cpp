#include "Face.hpp"

void Face::draw(sf::RenderTarget &window) {
	float initialX = 400;
	float initialY = 300;

	// eyes
	sf::CircleShape eye(10.0, 30.0);
	eye.setPosition(initialX - 40.0, initialY - 100.0);
	eye.setFillColor(sf::Color::White);
	window.draw(eye);
	eye.setPosition(initialX + 40.0, initialY - 100.0);
	window.draw(eye);

	// smile
	const int POINT_COUNT = 10;
	sf::Vertex points[POINT_COUNT];
	auto hMax = -40.0 * frown;
	auto hMin = 0.0;
	auto width = (float)(POINT_COUNT - 1);
	auto a = 4.0 * (hMax - hMin) / (width * width);

	for (int i = 0; i < POINT_COUNT; i++) {
		auto xCentered = ((float)i) - width / 2.0;
		auto y = a * xCentered * xCentered + hMin;
		points[i] = sf::Vertex(sf::Vector2f(initialX + (xCentered * width), initialY + y));
	}

	window.draw(points, POINT_COUNT, sf::LineStrip);
}
