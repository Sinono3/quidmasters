#pragma once
#include "GameState.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

struct Store {
	struct Item {
		std::string name;
		int price;
		std::string description;
		// std::function<void()> func;
		Gun gun;
	};
	std::vector<bool> bought;

  public:
	std::vector<Item> items;
	std::optional<int> hoveredOn;

	Store();
	void update(sf::RenderWindow &window, GameState& state);
	void render(sf::RenderWindow &window, sf::Font& font);
};
