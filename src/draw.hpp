#pragma once
#include "GameState.hpp"
#include "systems.hpp"
#include <SFML/Graphics.hpp>

struct DrawContext {
	sf::RenderWindow &window;
	const sf::Font &font;
	float time;
};

namespace draw {
void game(const GameState &state, FrameContext &frame, DrawContext &context);
void gameOver(const GameState &state, DrawContext &context);
void statusbar(DrawContext &ctx, const char *label, float var, float max,
			   float x, float y, float maxWidth, float height, sf::Color fill,
			   sf::Color bg);
void store(const GameState &state, DrawContext &context);
} // namespace draw
