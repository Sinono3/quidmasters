#include <SFML/Graphics.hpp>
#include "../draw.hpp"

void draw::pausedOverlay(DrawContext &ctx) {
	auto screenSize = GameDef::SCREEN_SIZE.to<float>().toSFML();

	// Darkening overlay
	sf::RectangleShape rect(screenSize);
	rect.setFillColor(sf::Color(0, 0, 0, 200));
	ctx.window.draw(rect);

	// Paused text
	sf::Text text;
	text.setFont(ctx.assets.papyrus);
	text.setString("             PAUSED\n press escape to unpause");

	// Center text on screen
	sf::Vector2f size = text.getLocalBounds().getSize();
	text.setPosition((screenSize / 2.0f) - (size / 2.0f));
	ctx.window.draw(text);
}
