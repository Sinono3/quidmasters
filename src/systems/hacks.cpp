#include "../systems.hpp"

void systems::hacks(GameState &state, const FrameContext &frame) {
	if (!state.hacksON 
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)
	) {
		state.player.coins = 10000;
		state.hacksON = true;
		state.setMessage(Message("bingo", 1.0f));
	}

	// Kill all enemies rn
	if (!state.inBreak
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)
	) {
		int totalQuidGenerated = 0;

		// Kill off spawned enemies and add their quid
		for (auto enemy : state.enemies) {
			totalQuidGenerated += enemy.calculateRandomQuidDrop(frame.rng);
		}
		state.enemies.clear();

		// Kill off not yet spawned enemies
		for (int i = 0; i < state.enemyClassCount.size(); i++) {
			auto enemy = GameDef::ENEMY_CLASSES[i].produce();

			for (int j = state.spawnedEnemyClassCount[i]; j < state.enemyClassCount[i]; j++) {
				totalQuidGenerated += enemy.calculateRandomQuidDrop(frame.rng);
			}

			state.spawnedEnemyClassCount[i] = state.enemyClassCount[i];
		}

		state.player.coins += totalQuidGenerated;
		state.player.nourishment = state.player.maxNourishment;
		state.setMessage(Message("kill 'em alll!!!!'", 1.0f));
	}

	// Skip break time
	if (state.inBreak
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)
	    && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)
	) {
		state.breakTime = 0.0f;
	}
}
