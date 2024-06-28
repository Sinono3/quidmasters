#pragma once
#include "GameState.hpp"
#include <SFML/Graphics.hpp>

struct DrawContext {
 sf::RenderWindow& window;
 const sf::Font& font;
 float time;
};

namespace draw {
 void gameOver(const GameState& state, DrawContext& context);
}
