#pragma once
#include "GameState.hpp"
#include "Assets.hpp"
#include "systems.hpp"
#include "menu.hpp"
#include <SFML/Graphics.hpp>

struct DrawContext {
	sf::RenderWindow &window;
	const Assets& assets;
	float time;
	FrameContext& frame;
	sf::Transform gameToScreen;
};

namespace draw {
void game(const GameState &state, FrameContext &frame, DrawContext &ctx);
void gameOver(const GameState &state, DrawContext &ctx);
void store(const GameState &state, DrawContext &ctx);
void pausedOverlay(DrawContext& ctx);
void menu(const menu::State& menu, DrawContext& ctx);

// Individual items
void player(DrawContext& ctx, const Player& player, Assets::Textures::Icon gunIcon);
void fog(DrawContext& ctx, float notoriety, float t);
void statusbar(DrawContext &ctx, const char *label, float var, float max,
			   float x, float y, float maxWidth, float height, sf::Color fill,
			   sf::Color bg);
void face(DrawContext &ctx, Vector2f position, float headRadius, float frown, float eyeRadius);
} // namespace draw
