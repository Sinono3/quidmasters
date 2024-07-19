#pragma once
#include "Assets.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <optional>
#include <memory>

struct Store {
	class PurchaseAction {
		public:
			// Because of circular dependency troubles.
			// I have to send a void pointer
			virtual void apply(void* state) = 0;
	};

	struct Item {
		std::string_view name;
		int price;
		std::string_view description;
		// Action to be applied to the game when purchasing the item
		std::unique_ptr<PurchaseAction> action;
		Assets::Textures::Icon icon;
	};


	// defined in `GameDef.cpp`
	static const float ITEM_TILE_SIZE;
	static const float SLACK;
	static const std::array<Item, 7> ITEMS;

	std::array<bool, ITEMS.size()> bought = { false };
	std::optional<int> hoveredOn;
};
