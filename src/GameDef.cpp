#include "GameDef.hpp"
#include "Gun.hpp"
#include "Store.hpp"
#include "Player.hpp"

const int GameDef::SCREEN_WIDTH = 800;
const int GameDef::SCREEN_HEIGHT = 600;
const float GameDef::SCALE = 10.0f;

const float Player::RADIUS = 1.0f;
const float Player::RADIUS_SQ = Player::RADIUS * Player::RADIUS;

const float GameDef::WAVE_TIME = 30.0f;
const float GameDef::BREAK_TIME = 10.0f;

const Gun GUN_SHODDY_PISTOL = { .type = Gun::Type::Handgun, .firePeriod = UniVar(0.7f, 1.6f),
	.damage = UniVar(3.9f, 4.1f), .knockback = UniVar(0.0f, 0.2f), .bulletSpeed = UniVar(50.0f, 100.0f),
	.accuracy = UniVar(-0.2f, 0.2f), .bulletsPerFire = 1, };
const Gun GUN_OKAYISH_PISTOL = { .type = Gun::Type::Handgun, .firePeriod = UniVar(0.2f, 0.7f),
	.damage = UniVar(3.9f, 4.1f), .knockback = UniVar(0.0f, 0.2f), .bulletSpeed = UniVar(80.0f, 100.0f),
	.accuracy = UniVar(-0.02f, 0.02f), .bulletsPerFire = 1, };
const Gun GUN_RIGHTSIDE_SHOTGUN = { .type = Gun::Type::Shotgun, .firePeriod = UniVar(0.7f, 1.0f),
	.damage = UniVar(4.9f, 6.1f), .knockback = UniVar(0.0f, 0.4f), .bulletSpeed = UniVar(200.0f, 300.0f),
	.accuracy = UniVar(-0.02f, 1.0f), .bulletsPerFire = 5, };
const Gun GUN_MACHINE_BOY = { .type = Gun::Type::MachineGun, .firePeriod = UniVar(0.08f, 0.1f),
	.damage = UniVar(1.0f, 1.8f), .knockback = UniVar(0.0f, 0.15f), .bulletSpeed = UniVar(100.0f, 200.0f),
	.accuracy = UniVar(-0.02f, 0.02f), .bulletsPerFire = 2, };
const Gun GUN_GRAVKILLER = { .type = Gun::Type::Homing, .firePeriod = UniVar(0.08f, 0.1f),
	.damage = UniVar(1.0f, 1.8f), .knockback = UniVar(0.0f, 0.15f), .bulletSpeed = UniVar(10.0f, 20.0f),
	.accuracy = UniVar(-0.02f, 0.02f), .bulletsPerFire = 2, };
const Gun GUN_EXPLD = { .type = Gun::Type::Shotgun, .firePeriod = UniVar(8.08f, 12.1f),
	.damage = UniVar(5.0f, 4.8f), .knockback = UniVar(0.0f, 0.4f), .bulletSpeed = UniVar(100.0f, 200.0f),
	.accuracy = UniVar(-3.14f, 3.14f), .bulletsPerFire = 200, };
const std::vector<Gun> GameDef::INITIAL_GUNS{{ GUN_SHODDY_PISTOL }};

// Store constants
const float Store::ITEM_TILE_SIZE = 40.0f;
const float Store::SLACK = 10.0f;
const std::array<Store::Item, 5> Store::ITEMS{{
	Store::Item { "Okay-ish handgun",  50, "It works for now", GUN_OKAYISH_PISTOL },
	Store::Item { "Rightside boye",  200, "Shoots to the right, but shoots a lotta", GUN_RIGHTSIDE_SHOTGUN },
	Store::Item { "Machine Boy", 800, "Yeahhhh", GUN_MACHINE_BOY },
	Store::Item { "Gravkiller", 2000, "Kills the homing zombies", GUN_GRAVKILLER, },
	Store::Item { "EXPLD151", 3000, "explode the world", GUN_EXPLD },
}};

// All related to enemies
const std::array<EnemyClass, 3> GameDef::ENEMY_CLASSES{{
	// Standard zombie
	{
		.maxSpeed = 10.0f, .acceleration = 180.0f,
		.maxHealth = 10.0f,
		.radius = 1.0f,
		.damagePerSecond = 0.1f,
		.color = {0, 200, 0, 255},
	},
	// big zomba
	{
		.maxSpeed = 5.0f, .acceleration = 180.0f,
		.maxHealth = 40.0f,
		.radius = 2.0f,
		.damagePerSecond = 0.1f,
		.color = {100, 0, 100, 255},
	},
	// quick zomb
	{
		.maxSpeed = 80.0f, .acceleration = 180.0f,
		.maxHealth = 3.0f,
		.radius = 0.5f,
		.damagePerSecond = 0.1f,
		.color = {255, 0, 255, 255},
	},
}};

