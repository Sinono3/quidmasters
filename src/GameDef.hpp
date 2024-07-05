#pragma once
#include "Enemy.hpp"
#include "Gun.hpp"
#include <span>

struct GameDef {
	static const float SCALE;
	static const int SCREEN_WIDTH;
	static const int SCREEN_HEIGHT;
	static const float WAVE_TIME;
	static const float BREAK_TIME;

	// Starting arsenal
	static const std::vector<Gun> INITIAL_GUNS;

	static const std::array<EnemyClass, 3> ENEMY_CLASSES;
};

// Type that holds a counter for every enemy class
using EnemyClassCounter = std::array<int, GameDef::ENEMY_CLASSES.size()>;
