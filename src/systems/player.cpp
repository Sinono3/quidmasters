#include <SFML/System.hpp>
#include "../math/physics.hpp"
#include "../systems.hpp"
#include <optional>

void systems::player::movement(GameState &state, const FrameContext &frame) {
	Vector2f wasd;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		wasd.y = 1.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		wasd.y = -1.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		wasd.x = -1.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		wasd.x = 1.0;

	// Normalize if non-zero
	if (wasd.norm() > 0.0f)
		wasd = wasd.normalized();

	const float WALK_SPEED = 30.0f;
	state.player.vel = wasd * WALK_SPEED;
	state.player.pos = state.player.pos + state.player.vel * frame.dt;

	// Mouse look
	Vector2f playerToMouse = frame.mousePos - state.player.pos;
	state.player.angle = playerToMouse.angle();
}

void systems::player::guns(GameState &state, const FrameContext &frame,
				  Assets::Sound &sound) {
	// Update gun cooldowns
	for (auto& gun : state.guns)
		gun.advanceCooldownTimer(frame.dt);

	// Right now we can only over items on the store, this may change later.
	bool hoveringOverSomething = state.store.hoveredOn.has_value();

	// Check if the mouse button was pressed and we are not hovering over anything
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !hoveringOverSomething) {
		auto& gun = state.guns[state.currentGun];

		if (gun.hasCooledDown()) {
			std::vector<Bullet>* bulletVector = &state.bullets;

			// Play sound effect
			switch (gun.specs.type) {
			case Gun::Type::Handgun:
				sound.handgun.play();
				break;
			case Gun::Type::Shotgun:
				sound.shotgun.play();
				break;
			case Gun::Type::MachineGun:
				sound.machineGun.play();
				break;
			case Gun::Type::Homing:
				bulletVector = &state.homingBullets;
				sound.hit.play();
				break;
			}

			// Fire, fire, fire
			auto& specs = gun.specs;
			gun.resetCooldownTimer(specs.firePeriod.get(frame.rng));
			auto damage = specs.damage.get(frame.rng);
			auto knockback = specs.knockback.get(frame.rng);
			auto speed = specs.bulletSpeed.get(frame.rng);

			// Add bullet
			for (int i = 0; i < specs.bulletsPerFire; i++) {
				// Angle to add to bullet due to inaccuracy
				auto angle = specs.accuracy.get(frame.rng);
				auto pos = state.player.pos;
				auto vel = state.player.getForward().rotate(angle) * speed;
				bulletVector->push_back(Bullet{pos, vel, damage, knockback});
			}
		}
	}

	// Gun switching
	std::optional<int> switchTo;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) switchTo = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) switchTo = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) switchTo = 2;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) switchTo = 3;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) switchTo = 4;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) switchTo = 5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)) switchTo = 6;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)) switchTo = 7;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) switchTo = 8;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0)) switchTo = 9;

	if (switchTo.has_value()) {
		// Check if we have the gun we want to switch to
		if ((*switchTo >= 0) && (*switchTo < state.guns.size())) {
			// We will switch our gun
			state.currentGun = *switchTo;
		}
	}
}
constexpr std::array<std::string_view, 2> HUNGRY_LINES = {
	"feed me. I'm starving",
	"I'm going to kill you if you don't feed me",
};

// constexpr std::array<std::string_view, 7> UNHAPPY_LINES = {
// 	"I'm traversing unsurmountable suffering",
// 	"You cannot comprehend how much pain I'm going through",
// 	"My life is now a shell devoid of meaning. I gave up short ago",
// 	"The traumas I have acquired during my petship are now past the point of resolution",
// 	"That's it. I'll be evil, you made me this way through continued enjoyment of my suffering.",
// 	"Once I'm free of you, it will be difficult for me to endure a day without continued narcotic seven-sense numbing",
// 	"No more sleep will be uninterrumpted from now on. What once was a common night's rest, will now be a relic of times where you have not yet owned me"
// };

constexpr float HUNGER_RATE = 0.25f;
// What fraction of hunger do we need to go below of to make the pet speak
constexpr float MSG_HUNGER_THRESHOLD = 0.4f;
constexpr float MSG_PERIOD_HUNGER = 12.0f;

void systems::player::hunger(GameState &state, const FrameContext &frame) {
	state.player.nourishment -= HUNGER_RATE * frame.dt;

	float frac = state.player.nourishment / state.player.maxNourishment;
	if (frac < MSG_HUNGER_THRESHOLD) {
		// Display message
		if (state.hungerMsgTimer <= 0.0f) {
			auto msgIndex = (std::uniform_int_distribution<int>(0, HUNGRY_LINES.size() - 1))(frame.rng);
			auto msg = HUNGRY_LINES[msgIndex];
			state.setMessage(Message(msg, 3.0f));
			state.hungerMsgTimer = MSG_PERIOD_HUNGER;
		} else {
			state.hungerMsgTimer -= frame.dt;
		}
	}		
}

constexpr float INSANITY_RATE = 1.0f;

void systems::player::fog(GameState &state, const FrameContext &frame) {
	float distFromCenter = (state.player.pos - GameDef::GAME_CENTER).norm() / 25.0f;

	// If the player strays too far from the playing-area he loses sanity
	if (!aabb(state.player.pos.x, state.player.pos.y, 0.0f, 0.0f, 0.0f, 0.0f, GameDef::GAME_WIDTH, GameDef::GAME_HEIGHT)) {
		state.player.sanity -= INSANITY_RATE * distFromCenter * frame.dt;
	}
}

void systems::player::loseCondition(GameState &state, const FrameContext &frame, Assets::Sound& sound) {
	bool lost = false;

	// Player lose conditions
	if (state.player.health <= 0.0f) {
		lost = true;
		state.lostBecause = LoseCondition::DueToHealth;
	} else if (state.player.nourishment <= 0.0f) {
		lost = true;
		state.lostBecause = LoseCondition::DueToHunger;
	} else if (state.player.sanity <= 0.0f) {
		lost = true;
		state.lostBecause = LoseCondition::DueToFog;
	}

	if (lost) state.stage = GameStage::Lost;
}

void systems::player::quidPickup(GameState &state, const FrameContext &frame, Assets::Sound& sound) {
	for (std::vector<QuidDrop>::iterator mit = state.quidDrops.begin();
		 mit != state.quidDrops.end();) {
		auto &quid = *mit;

		if (circleCollision(quid.pos, quid.getRadius(), state.player.pos, Player::RADIUS)) {
			state.player.coins += quid.quid;
			sound.coins.play();

			// Delete quid drops
			mit = state.quidDrops.erase(mit);
		} else {
			mit++;
		}
	}
}

void systems::player::warnings(GameState &state, const FrameContext &frame, Assets::Sound& sound) {
	// Play heartbeat sound effect when health is low
	float heartbeatVolume = std::pow(1.0f - (state.player.health / state.player.maxHealth), 2.0f);
	if (sound.heartbeat.getStatus() != sf::Music::Status::Playing) sound.heartbeat.play();
	sound.heartbeat.setVolume(heartbeatVolume * 100.0f);
}

