#include "../draw.hpp"

void draw::statusbar(DrawContext& ctx,
				   const char *label, float var, float max, float x, float y,
				   float maxWidth, float height, sf::Color fill, sf::Color bg) {

	float fraction = var / max;
	sf::RectangleShape shape(sf::Vector2f(maxWidth, height));

	// Background
	shape.setPosition(sf::Vector2f(x, y));
	shape.setFillColor(bg);
	ctx.window.draw(shape);

	// Fill
	shape.setSize(sf::Vector2f(fraction * maxWidth, height));
	shape.setFillColor(fill);
	ctx.window.draw(shape);

	sf::Text text;
	text.setFont(ctx.font);
	text.setString(label);
	text.setPosition(x, y);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);

	if (fraction >= 0.0 && fraction < 0.1) fraction = 0.3;
	if (fraction <= 0.0 && fraction > -0.1) fraction = -0.3;
	sf::Color outlineColor = sf::Color(
	                                   (fill.r * fraction + bg.r  + (1.0f - fraction)* (1.0f - fraction)), 
	                                   (fill.g * fraction + bg.g  + (1.0f - fraction)* (1.0f - fraction)), 
	                                   (fill.b * fraction + bg.b  + (1.0f - fraction)* (1.0f - fraction))
	                               );
	float thickness = 0.2 + (1.0 / fraction);

	text.setOutlineColor(outlineColor);
	text.setOutlineThickness(thickness);
	ctx.window.draw(text);
}

