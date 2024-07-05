#pragma once
#include "Bullet.hpp"
#include "GameDef.hpp"
#include "Enemy.hpp"
#include "Gun.hpp"
#include "Player.hpp"
#include "Store.hpp"
#include "QuidDrop.hpp"
#include <string>
#include <vector>
#include <optional>

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
	constexpr Message(const std::string_view& text, float time) : text(text), time(time) {}
};

struct GameState {
	// Meta-variables
	GameStage stage;
	LoseCondition lostBecause;

	// Entities
	Player player;
	std::vector<Enemy> enemies;
	std::vector<Bullet> bullets;
	std::vector<Bullet> homingBullets;
	std::vector<QuidDrop> quidDrops;

	// Gun stuff
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
	float hungerMsgTimer = 0.0f;

	// HAX heheeheheh
	bool hacksON = false;

	GameState() {
		stage = GameStage::Playing;
		// Set the guns to be the ones in INITIAL_GUNS
		guns.assign(GameDef::INITIAL_GUNS.begin(), GameDef::INITIAL_GUNS.end());
	}

	void setMessage(Message message) {
		this->message = message;
	}
};
