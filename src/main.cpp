#include "Assets.hpp"
#include "GameState.hpp"
#include "draw.hpp"
#include "math/Vector2.hpp"
#include "math/aabb.hpp"
#include "GameDef.hpp"
#include "systems.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

std::default_random_engine rng = std::default_random_engine();

int main() {
    sf::RenderWindow window(sf::VideoMode(GameDef::SCREEN_WIDTH, GameDef::SCREEN_HEIGHT), "A lonely dungeon");
    window.setVerticalSyncEnabled(true);

	// Load the assets
	Assets assets;

	GameState state;
    Vector2f cameraPos;

	sf::Clock time;
	sf::Clock deltaClock;

	Vector2f screenSize (GameDef::SCREEN_WIDTH / GameDef::SCALE, GameDef::SCREEN_HEIGHT / GameDef::SCALE);
	Vector2f screenCenter = screenSize * (1.0f / 2.0f);
	state.player.pos = screenCenter;

	assets.sound.music_Phase1.play();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		Vector2i screenMousePos = Vector2i(sf::Mouse::getPosition(window));
		Vector2f mousePos = (screenMousePos.to<float>() * (1.0f / GameDef::SCALE)) + cameraPos - screenCenter;
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

        DrawContext drawCtx {
        	.window = window,
        	.time = time.getElapsedTime().asSeconds(),
        	.assets = assets,
        	.frame = frame
        };

		window.clear(sf::Color(3, 2, 2));
        switch (state.stage) {
        	case GameStage::Playing:
				systems::player::movement(state, frame);
				systems::player::guns(state, frame, assets.sound);
				systems::player::hunger(state, frame);
				systems::player::loseCondition(state, frame);
				systems::player::quidPickup(state, frame, assets.sound);

				// Bullets
				// One system for each type of bullet (to avoid putting a switch inside the system)
				systems::bullets::physics(state, frame, assets.sound, state.bullets);
				systems::bullets::physics(state, frame, assets.sound, state.homingBullets);
				systems::bullets::homing(state, frame);

				// Wave system and enemy spawning
				systems::waves(state, frame);

				// Enemy systems
				systems::enemy::ai(state, frame);
				systems::enemy::collision(state, frame);
				systems::enemy::death(state, frame, assets.sound);

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
