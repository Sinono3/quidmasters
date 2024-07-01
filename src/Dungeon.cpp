#include "Face.hpp"

#include "GameSound.hpp"
#include "GameState.hpp"
#include "Fog.hpp"
#include "draw.hpp"
#include "math/Vector2.hpp"
#include "Gun.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Store.hpp"
#include "aabb.hpp"
#include "Constants.hpp"
#include "systems.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

std::default_random_engine rng;

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "A lonely dungeon");
    window.setVerticalSyncEnabled(true);
	sf::Font font;
	GameSound sound;
	
	if (!font.loadFromFile("fonts/papyrus.ttf")) {
		std::cerr << "We're fucked!" << std::endl;
	}

	GameState state;
    Vector2f cameraPos;

	sf::Clock time;
	sf::Clock deltaClock;

	Vector2f screenSize (SCREEN_WIDTH / TILE_SIZE, SCREEN_HEIGHT / TILE_SIZE);
	Vector2f screenCenter = screenSize * (1.0f / 2.0f);
	state.player.pos = screenCenter;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        DrawContext drawCtx {
        	.window = window,
        	.font = font,
        	.time = time.getElapsedTime().asSeconds(),
        };

		Vector2i screenMousePos = Vector2i(sf::Mouse::getPosition(window));
		Vector2f mousePos = (screenMousePos.to<float>() * (1.0f / TILE_SIZE)) + cameraPos - screenCenter;
    	float dt = deltaClock.restart().asSeconds();

		FrameContext frame {
			 // Delta time
			 .dt = dt,
			 // In world space
			 .screenSize = screenSize, // Vector (not point)
			 .screenCenter = screenCenter, // Vector (not point)
			 .screenMousePos = screenMousePos,
			 .mousePos = mousePos,
			 .cameraPos = cameraPos,
			 .rng = rng
		};

		window.clear(sf::Color(3, 2, 2));
        switch (state.stage) {
        	case GameStage::Playing:
				systems::player::movement(state, frame);
				systems::player::guns(state, frame, sound);
				systems::player::hunger(state, frame);
				systems::player::loseCondition(state, frame);

				// Bullets
				systems::bullets(state, frame);
				// Wave system and enemy spawning
				systems::waves(state, frame);

				// Enemy systems
				systems::enemy::ai(state, frame);
				systems::enemy::collision(state, frame);
				systems::enemy::death(state, frame, sound);

				// Store stuff (hover on item, buy items)
				systems::store(state, frame);

				// Update camera pos
				cameraPos = screenCenter;

				draw::game(state, frame, drawCtx);
        		break;
    		case GameStage::Lost:
	        	// Restart with enter
	        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
	        		state = GameState();
	        		state.player.pos = screenCenter;
	        	}

	        	draw::gameOver(state, drawCtx);
    			break;
    	}
		window.display();
    }

    return 0;
}
