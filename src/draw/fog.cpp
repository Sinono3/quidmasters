#include <SFML/Graphics.hpp>
#include "../draw.hpp"

void draw::fog(DrawContext& ctx, float notoriety, float t) {
	constexpr int FOGQ_X = 10;
	constexpr int FOGQ_Y = 10;
	// TODO: Debug purpose
	// notoriety = std::sin(t / 10.0f)  / 2.0f + 1.0f;
	sf::Sprite fogSprite(ctx.assets.textures.fog);

	for (int i = 0; i < FOGQ_Y; i++) {
		for (int j = 0; j < FOGQ_X; j++) {
			// int frame = (int)(t * 5.0f + y + x) % 4;
			// int tileX = frame % 2;
			// int tileY = frame / 2;
			float x = (-400.0f) + i * 100.0f;
			float y = (-400.0f) + j * 50.0f;
			// Displacement
			x += j * 30.0f * std::sin(((20.0f + t + i + j) / 10.0f));
			y += i * 30.0f * std::cos(t / j);
			fogSprite.setPosition(sf::Vector2f(x, y));
			fogSprite.setColor(sf::Color(255, 255, 255, notoriety * 100.0f));
			// fogSprite.setTextureRect(sf::IntRect(tileX * 174, tileY * 174, 174, 174));
			// fogSprite.setColor();
			ctx.window.draw(fogSprite);
		}
	}

	sf::Text theFogIsComing(ctx.assets.papyrus);
	theFogIsComing.setCharacterSize(30.0f);
	theFogIsComing.setString("the fog is coming ");
	theFogIsComing.setFillColor(sf::Color(255, 255, 255, notoriety * 50.0f));
	auto size = theFogIsComing.getGlobalBounds().size;

	for (int y = -size.y; y < GameDef::SCREEN_HEIGHT; y += size.y) {
		for (int x = -size.x; x < GameDef::SCREEN_WIDTH; x += size.x) {
			auto valueX = (int)((y + t) * 100.0f + y) % (int)size.x;
			// auto valueY = (int)(t * 50.0f) % (int)size.y;
			auto valueY = 0.0f;
			theFogIsComing.setPosition(sf::Vector2f(x + valueX, y + valueY));
			ctx.window.draw(theFogIsComing);
		}
	}
}
