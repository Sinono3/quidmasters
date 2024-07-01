#include <SFML/System.hpp>
#include "../aabb.hpp"
#include "../systems.hpp"
#include <optional>

void systems::player::movement(GameState &state, const FrameContext &frame) {
	Vector2f wasd;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		wasd.y = -1.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		wasd.y = 1.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		wasd.x = -1.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		wasd.x = 1.0;

	// Normalize if non-zero
	if (wasd.norm() > 0.0f)
		wasd = wasd.normalized();

	const float WALK_SPEED = 30.0f;
	Vector2f velocity = wasd * WALK_SPEED;
	state.player.pos = state.player.pos + velocity * frame.dt;

	// Mouse look
	Vector2f playerToMouse = frame.mousePos - state.player.pos;
	state.player.angle = atan2(playerToMouse.y, playerToMouse.x);
}

void systems::player::guns(GameState &state, const FrameContext &frame,
				  GameSound &sound) {
	// Right now we can only over items on the store, this may change later.
	bool hoveringOverSomething = state.store.hoveredOn.has_value();

	// Check if the mouse button was pressed and we are not hovering over anything
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !hoveringOverSomething) {
		if (state.gunCooldown.getElapsedTime().asSeconds() > state.gunCooldownTime) {
			state.gunCooldown.restart();

			// Fire, fire, fire
			auto& gun = state.guns[state.currentGun];
			state.gunCooldownTime = gun.firePeriod.get(frame.rng);
			auto damage = gun.damage.get(frame.rng);
			auto knockback = gun.knockback.get(frame.rng);
			auto speed = gun.bulletSpeed.get(frame.rng);

			// Add bullet
			for (int i = 0; i < gun.bulletsPerFire; i++) {
				// Angle to add to bullet due to inaccuracy
				auto angle = gun.accuracy.get(frame.rng);
				auto pos = state.player.pos;
				auto vel = state.player.getForward().rotate(angle) * speed;
				state.bullets.push_back(Bullet{pos, vel, damage, knockback});
			}

			// Play sound effect
			switch (gun.type) {
			case Gun::Type::Handgun:
				sound.handgun.play();
				break;
			case Gun::Type::Shotgun:
				sound.shotgun.play();
				break;
			case Gun::Type::MachineGun:
				sound.machineGun.play();
				break;
			}
		}
	}

	// Gun switching
	std::optional<int> switchTo;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) switchTo = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) switchTo = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) switchTo = 2;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) switchTo = 3;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) switchTo = 4;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) switchTo = 5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) switchTo = 6;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) switchTo = 7;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) switchTo = 8;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)) switchTo = 9;

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
			state.setMessage(Message(Message::Type::Hunger, msg, 3.0f));
			state.hungerMsgTimer = MSG_PERIOD_HUNGER;
		} else {
			state.hungerMsgTimer -= frame.dt;
		}
	}		
}
void systems::player::loseCondition(GameState &state, const FrameContext &frame) {
	// Player lose conditions
	if (state.player.health <= 0.0f) {
		state.stage = Lost;
		state.lostBecause = DueToHealth;
		return;
	}
	if (state.player.nourishment <= 0.0f) {
		state.stage = Lost;
		state.lostBecause = DueToHunger;
		return;
	}
	if (state.player.sanity <= 0.0f) {
		state.stage = Lost;
		state.lostBecause = DueToFog;
		return;
	}
}

void systems::player::quidPickup(GameState &state, const FrameContext &frame, GameSound& sound) {
	for (std::vector<QuidDrop>::iterator mit = state.quidDrops.begin();
		 mit != state.quidDrops.end();) {
		auto &quid = *mit;

		if (aabb(quid.pos.x, quid.pos.y, 0.1f, 0.1f,
				 state.player.pos.x - RADIUS, state.player.pos.y - RADIUS,
				 RADIUS * 2.0f, RADIUS * 2.0f)) {
			state.player.coins += quid.quid;
			sound.coins.play();

			// Delete quid drops
			mit = state.quidDrops.erase(mit);
		} else {
			mit++;
		}
	}
}
