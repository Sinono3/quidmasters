#include "../draw.hpp"
#include "../GameState.hpp"
#include <string>

void draw::gameOver(const GameState& state, DrawContext& context) {
	auto& window = context.window;
	auto& font = context.assets.papyrus;

	// Game over screen
	std::string subText;

	switch (state.lostBecause) {
	case LoseCondition::DueToHealth:
		subText = "You and your pet died.";
		break;
	case LoseCondition::DueToHunger:
		subText = "Your pet famished and ate you.";
		break;
	case LoseCondition::DueToFog:
		subText = "...";
		break;
	}
	subText.append("\nPress Enter to restart.");

	sf::Text title, subtitle;
	title.setString("You frickin lost!");
	title.setPosition(200.0f, 200.0f);
	title.setFillColor(sf::Color::Red);
	title.setFont(font);
	subtitle.setPosition(200.0f, 300.0f);
	subtitle.setString(subText);
	subtitle.setFillColor(sf::Color::White);
	subtitle.setFont(font);

	window.clear(sf::Color(12, 2, 2));
	window.draw(title);
	window.draw(subtitle);
}
