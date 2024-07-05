#pragma once
#include "Gun.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <optional>

struct Store {
	struct Item {
		std::string_view name;
		int price;
		std::string_view description;
		Gun gun;
	};

	// defined in `GameDef.cpp`
	static const float ITEM_TILE_SIZE;
	static const float SLACK;
	static const std::array<Item, 5> ITEMS;

	std::array<bool, ITEMS.size()> bought = { false };
	std::optional<int> hoveredOn;
};
