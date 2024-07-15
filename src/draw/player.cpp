#include "../draw.hpp"

void draw::player(DrawContext &ctx, const Player &player, Assets::Textures::Icon gunIcon) {
	sf::RenderStates states;
	states.transform = ctx.gameToScreen.translate(player.pos.toSFML());

	// Vector2f angleVec = Vector2f(1.0f, 0.0f).rotate(player.angle);
	// sf::Vertex velLine[2];
	// velLine[0] = sf::Vertex(sf::Vector2f(), sf::Color::Red);
	// velLine[1] =
	// 	sf::Vertex((player.vel * 2.0f).toSFML(), sf::Color::Red);
	// ctx.window.draw(velLine, 2, sf::Lines, states);

	// sf::Vertex angleLine[2];
	// angleLine[0] = sf::Vertex(sf::Vector2f(), sf::Color::Green);
	// angleLine[1] =
	// 	sf::Vertex((angleVec * 2.0f).toSFML(), sf::Color::Green);
	// ctx.window.draw(angleLine, 2, sf::Lines, states);

	// Legs
	const float LEG_RADIUS = 0.4f;
	const float LEG_SEP = 0.5f;
	const float LEG_MAX_FWD_DIST = 1.0f;
	const float LEG_MAX_LAT_DIST = 0.6f;
	const float LEG_SPEED = 12.0f;

	Vector2f leg1Pos(0.0f, LEG_SEP);
	Vector2f leg2Pos(0.0f, -LEG_SEP);

	float speed = player.vel.norm();
	Vector2f normVel = player.vel * (1.0f / speed);

	// Forward movement
	auto forwardMovement = (Vector2f(1.0f, 0.0f).rotate(player.angle)).dot(normVel);
	leg1Pos.x += forwardMovement * LEG_MAX_FWD_DIST * std::sin(LEG_SPEED * ctx.time);
	leg2Pos.x += forwardMovement * LEG_MAX_FWD_DIST * std::sin(LEG_SPEED * ctx.time + 3.14f);

	// Lateral movement
	auto lateralMovement = (Vector2f(0.0f, 1.0f).rotate(player.angle)).dot(normVel);
	leg1Pos.y += lateralMovement * LEG_MAX_LAT_DIST * std::sin(LEG_SPEED * ctx.time);
	leg2Pos.y += lateralMovement * LEG_MAX_LAT_DIST * std::sin(LEG_SPEED * ctx.time + 1.8f);

	// std::cout << "Fwd: " << forwardMovement << "\t Lateral: " << lateralMovement
			  // << std::endl;

	// Rotate them for the player angle
	leg1Pos = leg1Pos.rotate(player.angle);
	leg2Pos = leg2Pos.rotate(player.angle);

	// Set the spheres on center of this point (not on their corner)
	leg1Pos = leg1Pos - Vector2f(LEG_RADIUS, LEG_RADIUS);
	leg2Pos = leg2Pos - Vector2f(LEG_RADIUS, LEG_RADIUS);

	sf::CircleShape leg(LEG_RADIUS, 10);
	leg.setFillColor(sf::Color::White);
	leg.setPosition(leg1Pos.toSFML());
	ctx.window.draw(leg, states);
	leg.setPosition(leg2Pos.toSFML());
	ctx.window.draw(leg, states);

	// Head
	sf::CircleShape head(Player::RADIUS, 20);
	head.setPosition(-Player::RADIUS, -Player::RADIUS);
	ctx.window.draw(head, states);

	// Draw gun
	// auto playerPos = states.transform.transformPoint(sf::Vector2f());
	// std::cout << "Player pos: " << playerPos.x << ", " << playerPos.y << std::endl;
	// // auto gunTransform = sf::Transform().translate(playerPos);
	// auto gunTransform = sf::Transform().translate(playerPos).rotate(-player.angle * (180.0f / M_PI));
	// auto& gunTex = ctx.assets.textures.icons.get(gunIcon);
	// sf::Sprite gun(gunTex);
	// gun.setScale(1.0f, 1.0f);
	// // gun.setPosition(1.0f, 0.0f);
	// ctx.window.draw(gun, sf::RenderStates(gunTransform));

	// Player hitbox overlay
	// sf::CircleShape hitbox(1.0f, 20);
	// hitbox.setPosition(-1.0f, -1.0f);
	// hitbox.setFillColor(sf::Color::Transparent);
	// hitbox.setOutlineColor(sf::Color::Yellow);
	// hitbox.setOutlineThickness(0.01f);
	// ctx.window.draw(hitbox, states);
}
