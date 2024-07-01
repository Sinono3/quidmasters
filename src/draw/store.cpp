#include "../draw.hpp"
#include "../Store.hpp"

void draw::store(const GameState &state, DrawContext &ctx) {
	sf::Text storeTitle;
	storeTitle.setFont(ctx.font);
	storeTitle.setFillColor(sf::Color(200, 200, 200, 100));
	storeTitle.setPosition(Store::SLACK, SCREEN_HEIGHT - Store::ITEM_TILE_SIZE - Store::SLACK - 40.0f);
	storeTitle.setString("store");
	ctx.window.draw(storeTitle);

	for (int i = 0; i < Store::ITEMS.size(); i++) {
		float x = i * Store::ITEM_TILE_SIZE + Store::SLACK;
		float y = SCREEN_HEIGHT - Store::ITEM_TILE_SIZE - Store::SLACK;

		// Draw store item
    	sf::RectangleShape shape;
    	shape.setSize(sf::Vector2f(Store::ITEM_TILE_SIZE, Store::ITEM_TILE_SIZE));
    	shape.setPosition(x, y);
    	shape.setFillColor(sf::Color(40, 10, 10));
    	shape.setOutlineColor(sf::Color(100, 40, 40));
    	shape.setOutlineThickness(2.0f);
    	ctx.window.draw(shape);

		if (state.store.bought[i]) {
			sf::RectangleShape shape(sf::Vector2f(Store::ITEM_TILE_SIZE, Store::ITEM_TILE_SIZE));
			shape.setPosition(x, y);
			shape.setFillColor(sf::Color(255, 255, 0, 80));
			shape.setOutlineColor(sf::Color(255, 255, 0, 120));
			shape.setOutlineThickness(2.0f);
			ctx.window.draw(shape);

			sf::Text buyText;
			buyText.setFont(ctx.font);
			buyText.setFillColor(sf::Color(255, 255, 0, 200));
			buyText.setPosition(x, y);
			buyText.setString("SOLD\nOUT");
			buyText.setCharacterSize(10);
			ctx.window.draw(buyText);
		} 

		if (state.store.hoveredOn.has_value() && state.store.hoveredOn.value() == i) {
			sf::RectangleShape shape(
				sf::Vector2f(Store::ITEM_TILE_SIZE, Store::ITEM_TILE_SIZE));
			shape.setPosition(x, y);
			shape.setFillColor(sf::Color(0, 255, 0, 80));
			shape.setOutlineColor(sf::Color(0, 255, 0, 120));
			shape.setOutlineThickness(2.0f);
			ctx.window.draw(shape);

			sf::Text buyText;
			buyText.setFont(ctx.font);
			buyText.setFillColor(sf::Color(255, 255, 0, 200));
			buyText.setPosition(x, y);
			buyText.setString("BUY");
			buyText.setCharacterSize(15);
			ctx.window.draw(buyText);
		}
	}

	// If the player hovered on an item show its tooltip
	if (state.store.hoveredOn.has_value()) {
		float textX = Store::ITEMS.size() * Store::ITEM_TILE_SIZE + 30.0f;
		int index = state.store.hoveredOn.value();
		const auto &item = Store::ITEMS[index];

		sf::Text text;
		text.setPosition(textX, SCREEN_HEIGHT - 100.0f);
		text.setString(std::string(item.name));
		text.setFont(ctx.font);
		text.setCharacterSize(30.0f);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(2.0f);
		ctx.window.draw(text);

		text.setPosition(textX, SCREEN_HEIGHT - 40.0f);
		text.setString(std::string(item.description));
		text.setFont(ctx.font);
		text.setCharacterSize(20.0f);
		ctx.window.draw(text);
	}
}
