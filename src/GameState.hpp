#pragma once
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Gun.hpp"
#include "Player.hpp"
#include <vector>

enum GameStage {
	Playing,
	Lost,
};

enum LoseCondition {
	DueToHealth,
	DueToHunger,
	DueToFog,
};

struct GameState {
	// Meta-variables
	GameStage stage;
	LoseCondition lostBecause;

	// Actual game stuff
	Player player;
	std::vector<Enemy> enemies;
	std::vector<Bullet> bullets;
	std::vector<Gun> guns;
	int currentGun = 0;
	sf::Clock gunCooldown;
	float gunCooldownTime = 0.0f;

	// Wave stuff
	int enemyClassCount[2];
	int wave = 0;
	bool inWave = false;
	float timeTillNext = 0.1f;

	GameState() {
		stage = GameStage::Playing;
		guns = {GUN_SHODDY_PISTOL, GUN_OKAYISH_PISTOL, GUN_RIGHTSIDE_SHOTGUN,
				GUN_MACHINE_BOY, GUN_EXPLO};
	}
};
