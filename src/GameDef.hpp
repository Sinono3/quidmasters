#pragma once
#include "Enemy.hpp"
#include "Gun.hpp"
#include <span>

struct GameDef {
	// Pixel constants
	static const int SCREEN_WIDTH;
	static const int SCREEN_HEIGHT;
	static const Vector2i SCREEN_SIZE;
	// Scale of game world relative to screen space
	static const float SCALE;

	// Game world constants
	static const float GAME_WIDTH;
	static const float GAME_HEIGHT;
	static const Vector2f GAME_SIZE;
	static const Vector2f GAME_CENTER;
	// Minimum and maximum coordinates for an entity in the game world
	static const float MIN_X;
	static const float MIN_Y;
	static const float MAX_X;
	static const float MAX_Y;

	static const float WAVE_TIME;
	static const float BREAK_TIME;

	static const std::vector<Gun> INITIAL_GUNS;

	static const std::array<EnemyClass, 3> ENEMY_CLASSES;
};

// Type that holds a counter for every enemy class
using EnemyClassCounter = std::array<int, GameDef::ENEMY_CLASSES.size()>;
