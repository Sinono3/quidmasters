#include "../systems.hpp"

void systems::hacks(GameState &state, const FrameContext &frame) {
 if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
  state.player.coins = 10000;
 }
}
