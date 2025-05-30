#include <sstream>
#include "../draw.hpp"
#include "../Store.hpp"

void draw::store(const GameState &state, DrawContext &ctx) {
	// Store only operates during break
	if (!state.inBreak) {
		return;
	}

	sf::Text storeTitle(ctx.assets.papyrus);
	storeTitle.setFillColor(sf::Color(200, 200, 200, 100));
	storeTitle.setPosition(sf::Vector2f(Store::SLACK, GameDef::SCREEN_HEIGHT - Store::ITEM_TILE_SIZE - Store::SLACK - 40.0f));
	storeTitle.setString("store");
	ctx.window.draw(storeTitle);

	for (int i = 0; i < Store::ITEMS.size(); i++) {
		auto& item = Store::ITEMS[i];

		float x = i * Store::ITEM_TILE_SIZE + Store::SLACK;
		float y = GameDef::SCREEN_HEIGHT - Store::ITEM_TILE_SIZE - Store::SLACK;

		// Draw store item
    	sf::RectangleShape shape;
    	shape.setSize(sf::Vector2f(Store::ITEM_TILE_SIZE, Store::ITEM_TILE_SIZE));
    	shape.setPosition(sf::Vector2f(x, y));
    	shape.setFillColor(sf::Color(40, 10, 10));
    	shape.setOutlineColor(sf::Color(100, 40, 40));
    	shape.setOutlineThickness(2.0f);
    	ctx.window.draw(shape);

		if (state.store.bought[i]) {
			sf::RectangleShape shape(sf::Vector2f(Store::ITEM_TILE_SIZE, Store::ITEM_TILE_SIZE));
			shape.setPosition(sf::Vector2f(x, y));
			shape.setFillColor(sf::Color(255, 255, 0, 80));
			shape.setOutlineColor(sf::Color(255, 255, 0, 120));
			shape.setOutlineThickness(2.0f);
			ctx.window.draw(shape);

			sf::Text buyText(ctx.assets.papyrus);
			buyText.setFillColor(sf::Color(255, 255, 0, 200));
			buyText.setPosition(sf::Vector2f(x, y));
			buyText.setString("SOLD\nOUT");
			buyText.setCharacterSize(10);
			ctx.window.draw(buyText);
		} else if (state.store.hoveredOn.has_value() && state.store.hoveredOn.value() == i) {
			sf::RectangleShape shape(
				sf::Vector2f(Store::ITEM_TILE_SIZE, Store::ITEM_TILE_SIZE));
			shape.setPosition(sf::Vector2f(x, y));
			shape.setFillColor(sf::Color(0, 255, 0, 80));
			shape.setOutlineColor(sf::Color(0, 255, 0, 120));
			shape.setOutlineThickness(2.0f);
			ctx.window.draw(shape);

			sf::Text buyText(ctx.assets.papyrus);
			buyText.setFillColor(sf::Color(255, 255, 0, 200));
			buyText.setPosition(sf::Vector2f(x, y));
			buyText.setString("BUY");
			buyText.setCharacterSize(15);
			ctx.window.draw(buyText);
		} else {
			sf::Text priceText(ctx.assets.papyrus);
			priceText.setFillColor(sf::Color(255, 255, 0, 200));
			priceText.setPosition(sf::Vector2f(x, y + 20));
			priceText.setString(std::to_string(item.price));
			priceText.setCharacterSize(15);
			ctx.window.draw(priceText);
		}

		// Draw gun sprite
		sf::Sprite sprite(ctx.assets.textures.icons.get(item.icon));
		sprite.setPosition(sf::Vector2f(x, y));
		ctx.window.draw(sprite);
	}

	// If the player hovered on an item show its tooltip
	if (state.store.hoveredOn.has_value()) {
		float textX = Store::ITEMS.size() * Store::ITEM_TILE_SIZE + 30.0f;
		int index = state.store.hoveredOn.value();
		const auto &item = Store::ITEMS[index];

		std::stringstream title;
		title << item.name << " (" << item.price << " quid)";
		std::stringstream description;
		description << item.description;


		sf::Text text(ctx.assets.papyrus);
		text.setPosition(sf::Vector2f(textX, GameDef::SCREEN_HEIGHT - 100.0f));
		text.setString(title.str());
		text.setCharacterSize(30.0f);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(2.0f);
		ctx.window.draw(text);
		text.setPosition(sf::Vector2f(textX, GameDef::SCREEN_HEIGHT - 40.0f));
		text.setString(description.str());
		text.setCharacterSize(20.0f);
		ctx.window.draw(text);
	}
}
