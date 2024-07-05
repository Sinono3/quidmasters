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
#include "math/aabb.hpp"
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
	rng = std::default_random_engine();
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "A lonely dungeon");
    window.setVerticalSyncEnabled(true);
	sf::Font font;
	sf::Texture backgroundParking;
	GameSound sound;
	
	if (!font.loadFromFile("fonts/papyrus.ttf") || !backgroundParking.loadFromFile("sprites/Level1.jpeg")) {
		std::cerr << "We're fucked!" << std::endl;
		return 1;
	}

	GameState state;
    Vector2f cameraPos;

	sf::Clock time;
	sf::Clock deltaClock;

	Vector2f screenSize (SCREEN_WIDTH / TILE_SIZE, SCREEN_HEIGHT / TILE_SIZE);
	Vector2f screenCenter = screenSize * (1.0f / 2.0f);
	state.player.pos = screenCenter;
	// state.guns = {GUN_GRAVKILLER};
	// state.wave = 80;
	sound.music_Phase1.play();

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
        	.backgroundParking = backgroundParking
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
			 .minX = -screenSize.x,
			 .minY = -screenSize.y,
			 .maxX = 2.0f * screenSize.x,
			 .maxY = 2.0f * screenSize.y,
			 .rng = rng,
		};

		window.clear(sf::Color(3, 2, 2));
        switch (state.stage) {
        	case GameStage::Playing:
				systems::player::movement(state, frame);
				systems::player::guns(state, frame, sound);
				systems::player::hunger(state, frame);
				systems::player::loseCondition(state, frame);
				systems::player::quidPickup(state, frame, sound);

				// Bullets
				// One system for each type of bullet (to avoid putting a switch inside the system)
				systems::bullets::physics(state, frame, sound, state.bullets);
				systems::bullets::physics(state, frame, sound, state.homingBullets);
				systems::bullets::homing(state, frame);

				// Wave system and enemy spawning
				systems::waves(state, frame);

				// Enemy systems
				systems::enemy::ai(state, frame);
				systems::enemy::collision(state, frame);
				systems::enemy::death(state, frame, sound);

				// Message system (make msgs disappear after a while)
				systems::message(state, frame);
				// Store stuff (hover on item, buy items)
				systems::store(state, frame);
				// uuumm ehhh.... tha HACK SYSTEM
				systems::hacks(state, frame);

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
