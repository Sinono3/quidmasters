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

	// Kill all enemies rn
	if (!state.inBreak
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::K)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::L)
	) {
		int totalQuidGenerated = 0;

		// Kill off spawned enemies and add their quid
		for (auto enemy : state.enemies) {
			totalQuidGenerated += enemy.calculateRandomQuidDrop(frame.rng);
		}
		state.enemies.clear();

		// Kill off not yet spawned enemies
		for (int i = 0; i < ENEMY_CLASS_COUNT; i++) {
			auto enemy = ENEMY_CLASSES[i].produce();

			for (int j = state.spawnedEnemyClassCount[i]; j < state.enemyClassCount[i]; j++) {
				totalQuidGenerated += enemy.calculateRandomQuidDrop(frame.rng);
			}

			state.spawnedEnemyClassCount[i] = state.enemyClassCount[i];
		}

		state.player.coins += totalQuidGenerated;
		state.player.nourishment = state.player.maxNourishment;
		state.setMessage(Message(Message::Type::Evil, "kill 'em alll!!!!'", 1.0f));
	}

	// Skip break time
	if (state.inBreak
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::H)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::S)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::K)
	) {
		state.breakTime = 0.0f;
	}
}
