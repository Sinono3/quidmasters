#include <sstream>
#include "../GameDef.hpp"
#include "../draw.hpp"
#include "../systems.hpp"
#include "../Store.hpp"

void draw::game(const GameState &state, FrameContext &frame, DrawContext &ctx) {
	sf::RenderStates worldRenderState;
	worldRenderState.transform = ctx.gameToScreen;

	// Draw background
	sf::Sprite sprite;
	sprite.setTexture(ctx.assets.textures.bgL1);
	ctx.window.draw(sprite);

	// Draw quid drops
	for (auto &quid : state.quidDrops) {
		auto radius = quid.getRadius();
		auto inset = Vector2f(-radius, -radius);
		sf::CircleShape circle(radius, 4);
		circle.setFillColor(sf::Color::Yellow);
		circle.setPosition((quid.pos + inset).toSFML());
		ctx.window.draw(circle, worldRenderState);
	}

	// Draw player
	// auto inset = Vector2f(-Player::RADIUS, -Player::RADIUS);
	// sf::CircleShape playerCircle(Player::RADIUS, 20);
	// playerCircle.setFillColor(sf::Color::White);
	// playerCircle.setPosition((state.player.pos + inset).toSFML());
	// ctx.window.draw(playerCircle, worldRenderState);
	draw::player(ctx, state.player, state.guns[state.currentGun].specs.icon);

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

	// Draw homing bullets
	for (auto &bullet : state.homingBullets) {
		sf::Vertex line[] = {
			sf::Vertex((bullet.pos - bullet.vel * 0.01f).toSFML(),
					   sf::Color::Cyan),
			sf::Vertex((bullet.pos).toSFML(), sf::Color::Blue)};
		ctx.window.draw(line, 2, sf::Lines, worldRenderState);
	}

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

		// Draw gun sprite
		sf::Sprite sprite;
		sprite.setTexture(ctx.assets.textures.icons.get(gun.specs.icon));
		sprite.setPosition(x, y);
		ctx.window.draw(sprite);

		// Draw gun number
		sf::Text text;
		text.setFont(ctx.assets.papyrus);
		text.setString(std::to_string(i + 1));
		text.setCharacterSize(20.0f);
		text.setPosition(x, y);

		// Reloading
		draw::statusbar(ctx, "time", gun.cooldownTimer, gun.cooldown, 
		                x + 0.0f,
						y + GUN_ICON_SIZE, GUN_ICON_SIZE, 10.0f, 
						sf::Color(100, 100, 100, 100),
						sf::Color(255, 0, 0, 100)
					);
		ctx.window.draw(text);
	}

	// Draw statusbars
	draw::statusbar(ctx, "health", state.player.health,
				  state.player.maxHealth, 0.0, 0.0, 140.0, 30.0,
				  sf::Color::Green, sf::Color::Red);
	draw::statusbar(ctx, "not-hunger", state.player.nourishment,
				  state.player.maxNourishment, 150.0, 0.0, 100.0, 32.0,
				  sf::Color::Yellow, sf::Color::Red);
	draw::statusbar(ctx, "sanity", state.player.sanity,
				  state.player.maxSanity, 280.0, 0.0, 100.0, 49.0,
				  sf::Color(100, 100, 100), sf::Color::Red);


	// Draw pet face

	{
		// Make face float around
		float t = ctx.time;
		float x = 720.0f + 5.0f * std::sin(t + 1.41f);
		float y = 90.0f + 2.0f * std::cos(t * 2.0f);

		// Frown is based on average of health and nourishment
		float howsItGoin =
			((state.player.health / state.player.maxHealth) +
			 (state.player.nourishment / state.player.maxNourishment)) /
			2.0f;
		// float crazy = 1.0f - (state.player.sanity / state.player.maxSanity);
		// float twitch = crazy * (std::sin(200.0f * ctx.time * crazy) + 0.1f * std::sin(10.0f * 3.14f * ctx.time * crazy));
		float frown = (howsItGoin * 3.0) - 2.0;

		// Eye radius inverse proportional to = Hunger level + Waves
		float waves = state.wave;
		float eyeRadius = 5.0f / std::max(1.0f, waves);

		// Head radius increase proportional to waves
		float headRadius = 50.0f * (1.0f + waves * 0.01f);
		draw::face(ctx, Vector2f(x, y), headRadius, frown, eyeRadius);
	}

	// Show coins
	{
		std::stringstream ss;
		ss << "Quid: ";
		ss << state.player.coins;
		sf::Text text;
		text.setFont(ctx.assets.papyrus);
		text.setString(ss.str());
		text.setPosition(450.0, 0.0);
		text.setCharacterSize(40);
		ctx.window.draw(text);
	}

	// Show wave
	{
		std::stringstream ss;

		if (!state.inBreak) {
			int totalEnemyCount = 0;
			int remainingEnemyCount = state.enemies.size();

			for (int i = 0; i < state.enemyClassCount.size(); i++)
				totalEnemyCount += state.enemyClassCount[i];

			ss << "Wave: " << state.wave << '\n'
			   << "Enemies left: " << remainingEnemyCount;
			draw::statusbar(ctx, ss.str().c_str(), (float)remainingEnemyCount,
							(float)totalEnemyCount, 640, 540, 100, 60,
							sf::Color::Green, sf::Color::Magenta);
		} else {
			ss << "breaktime";
			draw::statusbar(ctx, ss.str().c_str(), state.breakTime,
						  GameDef::BREAK_TIME, 670, 540, 130, 60, sf::Color::Yellow,
						  sf::Color::Black);
		}
	}

	// Show store items
	draw::store(state, ctx);

	// Show current pet message
	if (state.message.has_value()) {
		sf::Text text;
		text.setFont(ctx.assets.papyrus);
		text.setString((*state.message).text);
		text.setFillColor(sf::Color::Cyan);

		float width = text.getLocalBounds().getSize().x;

		sf::RenderStates st;
		sf::Transform t;
		float fact = 0.01f * std::sin(ctx.time * 20.0f);
		st.transform = t.translate(GameDef::SCREEN_WIDTH - width - 150.0f, 60.0f)
						   .scale(1.0f + fact, 1.0f + fact);
		ctx.window.draw(text, st);
	}

	// Fog
	float sanityFraction = state.player.sanity / state.player.maxSanity;
	float distFromCenter = (state.player.pos - GameDef::GAME_CENTER).norm();
	float notoriety = (std::max(0.0f, distFromCenter - 30.0f) / 500.0f) + (1.0f - sanityFraction) * 0.3f;
	draw::fog(ctx, notoriety, ctx.time);
}
