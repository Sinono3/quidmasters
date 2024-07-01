#pragma once
#include "GameState.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

class Store {
  public:
	struct Item {
		std::string name;
		int price;
		std::string description;
		// std::function<void()> func;
		Gun gun;
	};

	Store();
	void update(sf::RenderWindow &window, GameState &state);
	void render(sf::RenderWindow &window, const sf::Font &font);

  private:
	std::vector<Item> items;
	std::optional<int> hoveredOn;
};
