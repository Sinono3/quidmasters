#include "Player.hpp"

void Player::processInput(const sf::RenderWindow& window, float dt) {
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
		wasd = wasd * (1.0f / wasd.norm());

	const float WALK_SPEED = 30.0f;
	const float TILE_SIZE = 20.0f;

	Vector2f velocity = wasd * WALK_SPEED;
	pos = pos + velocity * dt;

       // Mouse look
	Vector2f mousePos = Vector2i(sf::Mouse::getPosition(window)).to<float>();
	Vector2f gameMousePos = mousePos * (1.0f / TILE_SIZE);
	Vector2f playerToMouse = gameMousePos - pos;
	angle = atan2(playerToMouse.y, playerToMouse.x);
}
