#pragma once
#include "Spawner.hpp"
#include <iostream>

void Spawner::update(GameState &state) {
// 	if (timer.getElapsedTime().asSeconds() > spawnTime) {
// 		timer.restart();

// 		const int SLACK = 100;
// 		std::uniform_int_distribution distX(SLACK,
// 											(int)state.screenSize.x - SLACK);
// 		std::uniform_int_distribution distY(SLACK,
// 											(int)state.screenSize.y - SLACK);

// 		std::cout << "Spawned guy" << std::endl;
// 		// // Spawn guy
// 		// auto guy = Guy { Vector2((float)distX(state.rng), (float)distY(state.rng)) };
// 		// state.guys.push_back(guy);
// 	}
}
