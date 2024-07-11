#include "GameDef.hpp"
#include "Gun.hpp"
#include "Store.hpp"
#include "Player.hpp"
#include "GameState.hpp"
#include <memory>

const int GameDef::SCREEN_WIDTH = 800;
const int GameDef::SCREEN_HEIGHT = 600;
const Vector2i GameDef::SCREEN_SIZE(SCREEN_WIDTH, SCREEN_HEIGHT);
const float GameDef::SCALE = 10.0f;

const float GameDef::GAME_WIDTH = SCREEN_WIDTH / SCALE;
const float GameDef::GAME_HEIGHT = SCREEN_HEIGHT / SCALE;
const Vector2f GameDef::GAME_SIZE(GAME_WIDTH, GAME_HEIGHT);
const Vector2f GameDef::GAME_CENTER = GAME_SIZE * (1.0f / 2.0f);

const float GameDef::MIN_X = -GAME_WIDTH;
const float GameDef::MIN_Y = -GAME_HEIGHT;
const float GameDef::MAX_X = 2.0f * GAME_WIDTH;
const float GameDef::MAX_Y = 2.0f * GAME_HEIGHT;

const float Player::RADIUS = 1.0f;
const float Player::RADIUS_SQ = Player::RADIUS * Player::RADIUS;

const float GameDef::WAVE_TIME = 30.0f;
const float GameDef::BREAK_TIME = 10.0f;

const Gun GUN_SHODDY_PISTOL = {
	.type = Gun::Type::Handgun,
	.icon = Assets::Textures::Icon::Pistol,
	.firePeriod = UniVar(0.7f, 1.6f),
	.damage = UniVar(3.9f, 4.1f),
	.knockback = UniVar(0.0f, 0.2f),
	.bulletSpeed = UniVar(50.0f, 100.0f),
	.accuracy = UniVar(-0.2f, 0.2f),
	.bulletsPerFire = 1,
};
const Gun GUN_OKAYISH_PISTOL = {
	.type = Gun::Type::Handgun,
	.icon = Assets::Textures::Icon::Revolver,
	.firePeriod = UniVar(0.2f, 0.7f),
	.damage = UniVar(3.9f, 4.1f),
	.knockback = UniVar(0.0f, 0.2f),
	.bulletSpeed = UniVar(80.0f, 100.0f),
	.accuracy = UniVar(-0.02f, 0.02f),
	.bulletsPerFire = 1,
};
const Gun GUN_RIGHTSIDE_SHOTGUN = {
	.type = Gun::Type::Shotgun,
	.icon = Assets::Textures::Icon::Shotgun,
	.firePeriod = UniVar(0.7f, 1.0f),
	.damage = UniVar(4.9f, 6.1f),
	.knockback = UniVar(0.0f, 0.4f),
	.bulletSpeed = UniVar(200.0f, 300.0f),
	.accuracy = UniVar(-1.0f, 0.02f),
	.bulletsPerFire = 5,
};
const Gun GUN_MACHINE_BOY = {
	.type = Gun::Type::MachineGun,
	.icon = Assets::Textures::Icon::MachineGun,
	.firePeriod = UniVar(0.08f, 0.1f),
	.damage = UniVar(1.0f, 1.8f),
	.knockback = UniVar(0.0f, 0.15f),
	.bulletSpeed = UniVar(100.0f, 200.0f),
	.accuracy = UniVar(-0.02f, 0.02f),
	.bulletsPerFire = 2,
};
const Gun GUN_GRAVKILLER = {
	.type = Gun::Type::Homing,
	.icon = Assets::Textures::Icon::Bazooka,
	.firePeriod = UniVar(0.2f, 0.3f),
	.damage = UniVar(1.0f, 1.8f),
	.knockback = UniVar(0.0f, 0.15f),
	.bulletSpeed = UniVar(10.0f, 20.0f),
	.accuracy = UniVar(-3.14f, 3.14f),
	.bulletsPerFire = 2,
};
const Gun GUN_EXPLD = {
	.type = Gun::Type::Shotgun,
	.icon = Assets::Textures::Icon::Grenade,
	.firePeriod = UniVar(8.08f, 12.1f),
	.damage = UniVar(5.0f, 4.8f),
	.knockback = UniVar(0.7f, 0.9f),
	.bulletSpeed = UniVar(100.0f, 200.0f),
	.accuracy = UniVar(-3.14f, 3.14f),
	.bulletsPerFire = 4000,
};
const std::vector<Gun> GameDef::INITIAL_GUNS{{GUN_SHODDY_PISTOL}};

// Actions for store items
namespace actions {
std::unique_ptr<Store::PurchaseAction> addGun(Gun gun) {
	struct AddGun : public Store::PurchaseAction { 
		Gun gun;
		AddGun(Gun gun) : gun(gun) {}
		void apply(void* ptr) override {
			auto& state = *(GameState*)ptr;
			state.guns.push_back(GunState(gun));
		}
	};
	return std::unique_ptr<Store::PurchaseAction>(new AddGun(gun));
}
std::unique_ptr<Store::PurchaseAction> heal() {
	struct RefillPlayerHealth : public Store::PurchaseAction {
		void apply(void *ptr) override {
			auto& state = *(GameState*)ptr;
			state.player.health = state.player.maxHealth;
		}
	};
	return std::unique_ptr<Store::PurchaseAction>(new RefillPlayerHealth());
}
std::unique_ptr<Store::PurchaseAction> healSanity() {
	struct RefillPlayerSanity : public Store::PurchaseAction {
		void apply(void *ptr) override {
			auto& state = *(GameState*)ptr;
			state.player.sanity = state.player.maxSanity;
		}
	};
	return std::unique_ptr<Store::PurchaseAction>(new RefillPlayerSanity());
}
}

// Store constants
const float Store::ITEM_TILE_SIZE = 40.0f;
const float Store::SLACK = 10.0f;
const std::array<Store::Item, 7> Store::ITEMS{{
	Store::Item { "Okay-ish handgun",  50, "It works for now", actions::addGun(GUN_OKAYISH_PISTOL), GUN_OKAYISH_PISTOL.icon },
	Store::Item { "Rightside boye",  200, "Shoots to the right, but shoots a lotta", actions::addGun(GUN_RIGHTSIDE_SHOTGUN), GUN_RIGHTSIDE_SHOTGUN.icon },
	Store::Item { "Machine Boy", 800, "Yeahhhh", actions::addGun(GUN_MACHINE_BOY), GUN_MACHINE_BOY.icon },
	Store::Item { "EXPLD151", 1000, "explode the world", actions::addGun(GUN_EXPLD), GUN_EXPLD.icon },
	Store::Item { "Gravkiller", 2000, "Chase 'em'", actions::addGun(GUN_GRAVKILLER), GUN_GRAVKILLER.icon },
	Store::Item { "Medkit", 500, "A single full health refill", actions::heal(), Assets::Textures::Icon::Medkit },
	Store::Item { "Self-help book", 700, "Remember what makes you you", actions::healSanity(), Assets::Textures::Icon::Book },
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

