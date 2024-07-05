#include <SFML/Graphics.hpp>
#include "../draw.hpp"
#include "../menu.hpp"

const sf::Color BG_COLOR = sf::Color(10, 10, 10, 255);
const sf::Color DESELECTED_COLOR = sf::Color(100, 100, 100);
const sf::Color SELECTED_COLOR = sf::Color(200, 100, 40);
const float SELECTED_THICKNESS = 2.0f;

void draw::menu(const menu::State& menu, DrawContext &ctx) {
	auto screenSize = GameDef::SCREEN_SIZE.to<float>().toSFML();

	// Background
	sf::RectangleShape rect(screenSize);
	rect.setFillColor(BG_COLOR);
	ctx.window.draw(rect);

	// Title logo
	sf::Text text;
	text.setFont(ctx.assets.papyrus);
	text.setString("Quidmasters: Petbuilder");
	text.setPosition(40, 40);
	ctx.window.draw(text);

	text.setOutlineColor(sf::Color(255, 0, 0));
	float selectedThickness = SELECTED_THICKNESS * (std::sin(ctx.time * 8.0f) + 1.0f) / 2.0f;

	// Options
	for (int i = 0; i < menu::OPTIONS.size(); i++) {
		auto& option = menu::OPTIONS[i];
		auto x = 40;
		auto y = 170 + i * 40;
		text.setPosition(x, y);
		text.setString(option.text);
		text.setFillColor((i == menu.selectedOption) ? SELECTED_COLOR : DESELECTED_COLOR);
		text.setOutlineThickness((i == menu.selectedOption) ? selectedThickness : 0.0f);
		ctx.window.draw(text);
	}

	draw::face(ctx, Vector2f(700.0f, 120.0f), 50.0f, 1.0f, 3.0f);
}
