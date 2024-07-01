#include <SFML/System.hpp>
#include "../systems.hpp"

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
void systems::player::hunger(GameState &state, const FrameContext &frame) {
	state.player.nourishment -= 0.25f * frame.dt;
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
