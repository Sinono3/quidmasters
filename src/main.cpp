#include "Assets.hpp"
#include "GameState.hpp"
#include "math/physics.hpp"
#include "menu.hpp"
#include "draw.hpp"
#include "math/Vector2.hpp"
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
	sf::Clock time;
	sf::Clock deltaClock;

	// Load the assets
	Assets assets;
	menu::State menu;
	GameState state;
	state.stage = GameStage::Menu;

	bool escapedPressedLastFrame = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // This was made constant for now
        Vector2f cameraPos(0.0f, GameDef::GAME_HEIGHT);
        // Game to screen transform
    	auto gameToScreen =
    		sf::Transform()
    			// Set an unit to be in game scale
    			.scale(GameDef::SCALE, GameDef::SCALE)
    			// Make the positive y-coordinate point upward, not downward.
    			// (this is to make all math more standard)
    			.scale(1.0f, -1.0f)
    			// Center the screen on the camera position
    			.translate((-cameraPos).toSFML());
    			// .translate((-frame.cameraPos + GameDef::GAME_CENTER).toSFML());
    	auto screenToGame = gameToScreen.getInverse();

		Vector2i screenMousePos(sf::Mouse::getPosition(window));
		Vector2f mousePos(screenToGame.transformPoint(screenMousePos.to<float>().toSFML()));

    	float dt = deltaClock.restart().asSeconds();

		FrameContext frame {
			 // Delta time
			 .dt = dt,
			 // In world space
			 .screenMousePos = screenMousePos,
			 .mousePos = mousePos,
			 .cameraPos = cameraPos,
			 .rng = rng,
		};

        DrawContext drawCtx {
        	.window = window,
        	.assets = assets,
        	.time = time.getElapsedTime().asSeconds(),
        	.frame = frame,
        	.gameToScreen = gameToScreen
        };

        // Check if escape was just pressed
        bool escapedPressed =  sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
        bool escapedJustPressed = escapedPressed && !escapedPressedLastFrame;
		escapedPressedLastFrame = escapedPressed;

		// update the game
        switch (state.stage) {
        	case GameStage::Playing:
        	    // Pause the game if escape is pressed
	        	if (escapedJustPressed) {
	        	    state.stage = GameStage::Paused;
	        	    break;
        	    }

				systems::player::movement(state, frame);
				systems::player::guns(state, frame, assets.sound);
				systems::player::hunger(state, frame);
				systems::player::fog(state, frame);
				systems::player::loseCondition(state, frame, assets.sound);
				systems::player::quidPickup(state, frame, assets.sound);
				systems::player::warnings(state, frame, assets.sound);

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
				systems::store(state, frame, assets.sound);
				// Play music
				systems::music(state, frame, assets.sound);
				// uuumm ehhh.... tha HACK SYSTEM
				systems::hacks(state, frame);
        		break;
        	case GameStage::Paused:
        	    // Unpause the game if escape is pressed
	        	if (escapedJustPressed) state.stage = GameStage::Playing;
				break;
        	case GameStage::Menu:
        	    // Check if an item is selected using the mouse
            	for (int i = 0; i < menu::OPTIONS.size(); i++) {
            		auto x = 40;
            		auto y = 170 + i * 40;
            		auto width = 100;
            		auto height = 40;

            	    if (aabb(x, y, width, height, screenMousePos.x, screenMousePos.y, 0, 0)) {
                	    // If the selection was made this frame
                	    if (i != menu.selectedOption) {
                	        assets.sound.menuSelect.play();
                    	    menu.selectedOption = i;
                	    }
        	            break;
            	    }
        	    }


        	    // Check if user clicked on
        	    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && menu.selectedOption != -1) {
        	        (menu::OPTIONS[menu.selectedOption].action)(state, menu);
        	    }
        	    break;
    		case GameStage::Lost:
	        	// Restart with enter
	        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
	        	    state = GameState();
	        	    state.stage = GameStage::Playing;
	        	}
    			break;
    	}

		
		// Draw the game
    	window.clear(sf::Color(3, 2, 2));
		switch (state.stage) {
        	case GameStage::Playing:
                draw::game(state, frame, drawCtx);
        		break;
        	case GameStage::Paused:
				draw::game(state, frame, drawCtx);
				// Draw overlay over the game
				draw::pausedOverlay(drawCtx);
				break;
        	case GameStage::Menu:
        	    draw::menu(menu, drawCtx);
        	    break;
    		case GameStage::Lost:
	        	draw::gameOver(state, drawCtx);
    			break;
    	}
		window.display();
    }

    return 0;
}
