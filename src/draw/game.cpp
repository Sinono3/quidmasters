#include <sstream>
#include "../Constants.hpp"
#include "../draw.hpp"
#include "../systems.hpp"
#include "../Face.hpp"
#include "../Store.hpp"

void draw::game(const GameState &state, FrameContext &frame,
				DrawContext &ctx) {
	// Screen size in game units
	auto cameraTransform =
		sf::Transform()
			.scale(TILE_SIZE, TILE_SIZE)
			.translate((-frame.cameraPos + frame.screenCenter).toSFML());

	// used for game world rendering
	sf::RenderStates worldRenderState;
	worldRenderState.transform = cameraTransform;

	// Draw player
	auto inset = Vector2f(-RADIUS, -RADIUS);
	sf::CircleShape playerCircle(RADIUS, 20);
	playerCircle.setFillColor(sf::Color::Green);
	playerCircle.setPosition((state.player.pos + inset).toSFML());
	ctx.window.draw(playerCircle, worldRenderState);

	// Draw player direction
	sf::Vertex line[] = {
		sf::Vertex(state.player.pos.toSFML(), sf::Color::Green),
		sf::Vertex((state.player.pos + state.player.getForward()).toSFML(),
				   sf::Color::Red)};
	ctx.window.draw(line, 2, sf::Lines, worldRenderState);

	// Draw enemies
	for (auto &enemy : state.enemies) {
		auto inset = Vector2f(-enemy.radius, -enemy.radius);
		sf::CircleShape circle(enemy.radius, 20);
		circle.setFillColor(enemy.color);
		circle.setPosition((enemy.pos + inset).toSFML());
		ctx.window.draw(circle, worldRenderState);
	}

	// Draw bullets
	for (auto &bullet : state.bullets) {
		sf::Vertex line[] = {
			sf::Vertex((bullet.pos - bullet.vel * 0.01f).toSFML(),
					   sf::Color::Green),
			sf::Vertex((bullet.pos).toSFML(), sf::Color::Red)};
		ctx.window.draw(line, 2, sf::Lines, worldRenderState);
	}

	// Fog fog;
	// fog.notoriety = (state.player.pos - screenCenter).norm() / 2000.0f;
	// fog.draw(window, font, time.getElapsedTime().asSeconds());

	// Show gun selection
	const float GUN_ICON_SIZE = 30.0f;
	for (int i = 0; i < state.guns.size(); i++) {
		auto &gun = state.guns[i];
		float x = i * GUN_ICON_SIZE;
		float y = 40.0f;

		sf::RectangleShape shape;
		shape.setSize(sf::Vector2f(GUN_ICON_SIZE, GUN_ICON_SIZE));
		shape.setPosition(x, y);

		auto fillColor = sf::Color(40, 10, 10);
		auto outlineColor = sf::Color(100, 40, 40);

		if (state.currentGun == i) {
			fillColor = sf::Color(90, 60, 90);
			outlineColor = sf::Color(150, 90, 90);
		}

		shape.setFillColor(fillColor);
		shape.setOutlineColor(outlineColor);
		shape.setOutlineThickness(2.0f);
		ctx.window.draw(shape);

		// Draw gun number
		sf::Text text;
		text.setFont(ctx.font);
		text.setString(std::to_string(i + 1));
		text.setCharacterSize(20.0f);
		text.setPosition(x, y);
		ctx.window.draw(text);
	}

	// Draw statusbars
	draw::statusbar(ctx, "health", state.player.health,
				  state.player.maxHealth, 0.0, 0.0, 140.0, 30.0,
				  sf::Color::Green, sf::Color::Red);
	draw::statusbar(ctx, "not-hunger", state.player.nourishment,
				  state.player.maxNourishment, 150.0, 0.0, 100.0, 32.0,
				  sf::Color::Yellow, sf::Color::Red);
	draw::statusbar(ctx, "fog-awareness", state.player.sanity,
				  state.player.maxSanity, 330.0, 0.0, 100.0, 49.0,
				  sf::Color(100, 100, 100), sf::Color::Red);

	// Reloading
	float reload = std::min(state.gunCooldown.getElapsedTime().asSeconds(),
							state.gunCooldownTime);
	draw::statusbar(ctx, "time", reload, state.gunCooldownTime, 0.0f, 80.0f,
					30.0f, 20.0f, sf::Color(100, 100, 100), sf::Color::Red);

	// Draw pet face

	{
		// Make face float around
		float t = ctx.time;
		float x = 720.0f + 5.0f * std::sin(t + 1.41f);
		float y = 90.0f + 2.0f * std::cos(t * 2.0f);

		Face face(sf::Vector2f(x, y));
		float howsItGoin =
			((state.player.health / state.player.maxHealth) +
			 (state.player.nourishment / state.player.maxNourishment) +
			 (state.player.sanity / state.player.maxSanity)) /
			3.0f;
		face.frown = (howsItGoin * 3.0) - 2.0;
		face.draw(ctx.window);
	}

	// Show coins
	{
		std::stringstream ss;
		ss << "Quid: ";
		ss << state.player.coins;
		sf::Text text;
		text.setFont(ctx.font);
		text.setString(ss.str());
		text.setPosition(500.0, 0.0);
		text.setCharacterSize(40);
		ctx.window.draw(text);
	}

	// Show wave
	{
		std::stringstream ss;

		if (!state.inBreak) {
			int totalEnemyCount = 0;
			int remainingEnemyCount = state.enemies.size();
			for (int i = 0; i < ENEMY_CLASS_COUNT; i++)
				totalEnemyCount += state.enemyClassCount[i];

			ss << "Wave: ";
			ss << state.wave;
			draw::statusbar(ctx, ss.str().c_str(),
						  (float)remainingEnemyCount, (float)totalEnemyCount,
						  670, 540, 100, 60, sf::Color::Green,
						  sf::Color::Magenta);
		} else {
			ss << "breaktime";
			draw::statusbar(ctx, ss.str().c_str(), state.breakTime,
						  BREAK_TIME, 670, 540, 130, 60, sf::Color::Yellow,
						  sf::Color::Black);
		}
	}

	// Show store items
	Store store;
	store.render(ctx.window, ctx.font);
}
