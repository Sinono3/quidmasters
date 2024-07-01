#include "../systems.hpp"

void systems::hacks(GameState &state, const FrameContext &frame) {
	if (!state.hacksON 
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::B)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::I)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::N)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::G)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::O)
	) {
		state.player.coins = 10000;
		state.hacksON = true;
		state.setMessage(Message(Message::Type::Evil, "bingo", 1.0f));
	}
}
