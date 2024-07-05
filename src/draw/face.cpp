#include "../draw.hpp"

// Frown goes from -1 to 1, indicating how happy the face is
void draw::face(DrawContext &ctx, Vector2f position, float frown) {
	float initialX = position.x;
	float initialY = position.y;

	// head
	const float HEAD_R = 50.0f;
	sf::CircleShape head(HEAD_R, 30);
	head.setPosition(initialX - HEAD_R, initialY - HEAD_R - 30.0f);
	head.setFillColor(sf::Color::Black);
	head.setOutlineColor(sf::Color::White);
	head.setOutlineThickness(1.0f);
	ctx.window.draw(head);

	// eyes
	const float R = 5.0f;
	sf::CircleShape eye(R, 10.0f);
	eye.setPosition(initialX - 20.0 - R, initialY - 50.0 - R);
	eye.setFillColor(sf::Color::White);
	ctx.window.draw(eye);
	eye.setPosition(initialX + 20.0 - R, initialY - 50.0 - R);
	ctx.window.draw(eye);

	// smile
	const int POINT_COUNT = 10;
	sf::Vertex points[POINT_COUNT];
	auto hMax = -20.0f * frown;
	auto hMin = 0.0f;
	auto width = (float)(POINT_COUNT - 1);
	auto a = 4.0 * (hMax - hMin) / (width * width);

	for (int i = 0; i < POINT_COUNT; i++) {
		auto xCentered = ((float)i) - width / 2.0;
		auto y = a * xCentered * xCentered + hMin;
		points[i] = sf::Vertex(sf::Vector2f(initialX + (xCentered * width), initialY + y));
	}

	ctx.window.draw(points, POINT_COUNT, sf::LineStrip);
}
