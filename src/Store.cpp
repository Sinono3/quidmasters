#include "Store.hpp"
#include "Constants.hpp"
#include "GameState.hpp"
#include "Gun.hpp"
#include "aabb.hpp"
#include "math/Vector2.hpp"
#include <iostream>
#include <optional>

constexpr float STORE_ITEM_SIZE = 40.0f;
constexpr float SLACK = 10.0f;

void drawStoreItem(sf::RenderWindow &window, const Store::Item &item, float x,
				   float y) {
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(STORE_ITEM_SIZE, STORE_ITEM_SIZE));
	shape.setPosition(x, y);
	shape.setFillColor(sf::Color(40, 10, 10));
	shape.setOutlineColor(sf::Color(100, 40, 40));
	shape.setOutlineThickness(2.0f);
	window.draw(shape);
}

Store::Store() {
	items = std::vector<Store::Item>{
		Store::Item{
			"Okay-ish handgun", 50,
			"It works for now, but you're defintely get a better one soon",
			GUN_OKAYISH_PISTOL},
		Store::Item{"Rightside boye", 200,
					"Shoots to the right, but shoots a lotta",
					GUN_RIGHTSIDE_SHOTGUN},
		Store::Item{"Machine Boy", 1000, "Yeahhhh", GUN_MACHINE_BOY},
	};
	bought = std::vector<bool>{{0, 0, 0}};
}

void Store::update(sf::RenderWindow &window, GameState &state) {
	Vector2i mousePos(sf::Mouse::getPosition(window));

	hoveredOn.reset();
	for (int i = 0; i < items.size(); i++) {
		float x = i * STORE_ITEM_SIZE + SLACK;
		float y = SCREEN_HEIGHT - STORE_ITEM_SIZE - SLACK;
		// Check if the mouse is hovering on this item
		if (aabb(x, y, STORE_ITEM_SIZE, STORE_ITEM_SIZE, mousePos.x, mousePos.y, 0.0f, 0.0f)) {
			hoveredOn = i;
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (hoveredOn.has_value()) {
			int index = hoveredOn.value();
			auto &item = items[index];

			// if (!bought[index] && (state.player.coins >= item.price)) {
			// 	state.gun = item.gun;
			// 	bought[index] = true;
			// 	state.player.coins -= item.price;
			// 	// Buy the item!
			// 	std::cout << "Bought item!" << std::endl;
			// }
		}
	}
}

void Store::render(sf::RenderWindow &window, sf::Font &font) {
	sf::Text storeTitle;
	storeTitle.setFont(font);
	storeTitle.setFillColor(sf::Color(200, 200, 200, 100));
	storeTitle.setPosition(SLACK, SCREEN_HEIGHT - STORE_ITEM_SIZE - SLACK - 40.0f);
	storeTitle.setString("store");
	window.draw(storeTitle);

	for (int i = 0; i < items.size(); i++) {
		float x = i * STORE_ITEM_SIZE + SLACK;
		float y = SCREEN_HEIGHT - STORE_ITEM_SIZE - SLACK;
		drawStoreItem(window, items[i], x, y);

		if (bought[i]) {
			sf::RectangleShape shape(sf::Vector2f(STORE_ITEM_SIZE, STORE_ITEM_SIZE));
			shape.setPosition(x, y);
			shape.setFillColor(sf::Color(255, 255, 0, 80));
			shape.setOutlineColor(sf::Color(255, 255, 0, 120));
			shape.setOutlineThickness(2.0f);
			window.draw(shape);

			sf::Text buyText;
			buyText.setFont(font);
			buyText.setFillColor(sf::Color(255, 255, 0, 200));
			buyText.setPosition(x, y);
			buyText.setString("SOLD\nOUT");
			buyText.setCharacterSize(10);
			window.draw(buyText);
		} 

		if (hoveredOn.has_value() && hoveredOn.value() == i) {
			sf::RectangleShape shape(
				sf::Vector2f(STORE_ITEM_SIZE, STORE_ITEM_SIZE));
			shape.setPosition(x, y);
			shape.setFillColor(sf::Color(0, 255, 0, 80));
			shape.setOutlineColor(sf::Color(0, 255, 0, 120));
			shape.setOutlineThickness(2.0f);
			window.draw(shape);

			sf::Text buyText;
			buyText.setFont(font);
			buyText.setFillColor(sf::Color(255, 255, 0, 200));
			buyText.setPosition(x, y);
			buyText.setString("BUY");
			buyText.setCharacterSize(15);
			window.draw(buyText);
		}
	}

	// If the player hovered on an item show its tooltip
	if (hoveredOn.has_value()) {
		float textX = items.size() * STORE_ITEM_SIZE + 30.0f;
		int index = hoveredOn.value();
		auto &item = items[index];

		sf::Text text;
		text.setPosition(textX, SCREEN_HEIGHT - 100.0f);
		text.setString(item.name);
		text.setFont(font);
		text.setCharacterSize(30.0f);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(2.0f);
		window.draw(text);

		text.setPosition(textX, SCREEN_HEIGHT - 40.0f);
		text.setString(item.description);
		text.setFont(font);
		text.setCharacterSize(20.0f);
		window.draw(text);
	}
}
