#pragma once
#include "GameState.hpp"
#include <functional>
#include <iostream>
#include <string>

namespace menu {
struct State {
	int selectedOption = -1;
};

struct Option {
	std::string text;
	std::function<void(GameState &, State &)> action;
};

const std::vector<Option> OPTIONS{{
	Option{"Play",
		   [](auto& game, auto& menu) {
			   game = GameState();
			   game.stage = GameStage::Playing;
		   }},
	Option{"Quid", [](auto& game, auto& menu) { exit(0); }},
}};
} // namespace menu
