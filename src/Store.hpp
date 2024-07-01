#pragma once
#include "Gun.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

struct Store {
	struct Item {
		std::string_view name;
		int price;
		std::string_view description;
		Gun gun;
	};

	static constexpr float ITEM_TILE_SIZE = 40.0f;
	static constexpr float SLACK = 10.0f;
	static constexpr std::array<Item, 3> ITEMS{{
		Store::Item {
			"Okay-ish handgun", 
			50,
			"It works for now, but you're defintely get a better one soon",
			GUN_OKAYISH_PISTOL
		},
		Store::Item {
			"Rightside boye", 
			200,
			"Shoots to the right, but shoots a lotta",
			GUN_RIGHTSIDE_SHOTGUN
		},
		Store::Item {
			"Machine Boy",
			 1000,
			 "Yeahhhh",
			 GUN_MACHINE_BOY
		},
	}};

	std::array<bool, ITEMS.size()> bought = { false };
	std::optional<int> hoveredOn;
};
