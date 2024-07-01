#pragma once
#include "Bullet.hpp"
#include "Constants.hpp"
#include "Enemy.hpp"
#include "Gun.hpp"
#include "Player.hpp"
#include "Store.hpp"
#include <string>
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

struct Message {
	std::string text;
	float time;
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

	// Store state
	Store store;

	// Wave stuff
	EnemyClassCounter enemyClassCount;
	EnemyClassCounter spawnedEnemyClassCount;
	float enemySpawnTime = 1.0f;
	int wave = 0;
	bool inBreak = true;
	float breakTime = 1.0f;

	// Miscellaneous state
	std::optional<Message> message;

	GameState() {
		stage = GameStage::Playing;
		guns = {GUN_SHODDY_PISTOL, GUN_OKAYISH_PISTOL, GUN_RIGHTSIDE_SHOTGUN,
				GUN_MACHINE_BOY, GUN_EXPLO};
	}
};
