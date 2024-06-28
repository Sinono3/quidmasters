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
#include <vector>
#include <algorithm>
#include <sstream>

std::default_random_engine rng;

void drawStatusBar(sf::RenderTarget &window, const sf::Font &font,
				   const char *label, float var, float max, float x, float y,
				   float maxWidth, float height, sf::Color fill, sf::Color bg) {

	float fraction = var / max;
	sf::RectangleShape shape(sf::Vector2f(maxWidth, height));

	// Background
	shape.setPosition(sf::Vector2f(x, y));
	shape.setFillColor(bg);
	window.draw(shape);

	// Fill
	shape.setSize(sf::Vector2f(fraction * maxWidth, height));
	shape.setFillColor(fill);
	window.draw(shape);

	sf::Text text;
	text.setFont(font);
	text.setString(label);
	text.setPosition(x, y);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::Black);

	if (fraction >= 0.0 && fraction < 0.1) fraction = 0.1;
	if (fraction <= 0.0 && fraction > -0.1) fraction = -0.1;
	sf::Color outlineColor = sf::Color(
	                                   (fill.r * fraction + bg.r  + (1.0f - fraction)* (1.0f - fraction)), 
	                                   (fill.g * fraction + bg.g  + (1.0f - fraction)* (1.0f - fraction)), 
	                                   (fill.b * fraction + bg.b  + (1.0f - fraction)* (1.0f - fraction))
	                               );
	float thickness = 0.2 + (1.0 / fraction) * 1.0;

	text.setOutlineColor(outlineColor);
	text.setOutlineThickness(thickness);
	window.draw(text);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "A lonely dungeon");
    window.setVerticalSyncEnabled(true);
	sf::Font font;
	GameSound sound;
	
	if (!font.loadFromFile("fonts/papyrus.ttf")) {
		std::cerr << "We're fucked!" << std::endl;
	}

	const float RADIUS = 1.0f;

	GameState state;
	Store store;
	// This variable is set when the player loses
	Fog fog;
    Vector2f cameraPos;

	sf::Clock time;
	sf::Clock deltaClock;

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

		const float TILE_SIZE = 10.0f;
		Vector2f screenSize (SCREEN_WIDTH / TILE_SIZE, SCREEN_HEIGHT / TILE_SIZE);
		Vector2f screenCenter = screenSize * (1.0f / 2.0f);
		Vector2f screenMousePos = Vector2i(sf::Mouse::getPosition(window)).to<float>();
		Vector2f mousePos = (screenMousePos * (1.0f / TILE_SIZE)) + cameraPos - screenCenter;
    	float dt = deltaClock.restart().asSeconds();

		FrameContext frame {
			 // Delta time
			 .dt = dt,
			 // In world space
			 .screenSize = screenSize, // Vector (not point)
			 .screenCenter = screenCenter, // Vector (not point)
			 .mousePos = mousePos,
			 .cameraPos = cameraPos,
			 .rng = rng
		};

        switch (state.stage) {
        	case GameStage::Playing:
				player::movement(state, frame);
				player::guns(state, frame, sound);
				player::hunger(state, frame);
				player::loseCondition(state, frame);

				// Bullets
				bullets(state, frame);
				// Wave system and enemy spawning
				waves(state, frame);











				
				// Enemy AI
				for (auto& enemy : state.enemies) {
					// Movement
		    		Vector2f targetVel = (state.player.pos - enemy.pos).normalized() * enemy.maxSpeed;
		    		Vector2f velDiff = targetVel - enemy.vel;
		    		Vector2f accel = velDiff;
		    		if (velDiff.norm() > (enemy.acceleration * dt)) {
						accel = velDiff.normalized() * enemy.acceleration;
					} else {
						accel = velDiff;
					}
				    enemy.vel = enemy.vel + accel * dt;
		    		enemy.pos = enemy.pos + enemy.vel * dt;

		    		// Damage player
					if (aabb(state.player.pos.x - RADIUS, state.player.pos.y - RADIUS, RADIUS * 2.0f, RADIUS * 2.0f, enemy.pos.x - RADIUS, enemy.pos.y - RADIUS, RADIUS * 2.0f, RADIUS * 2.0f)) {
						state.player.health -= 0.1f;
					}
				}

				// Enemy collision
				for (int i = 0; i < state.enemies.size(); i++) {
					auto& a = state.enemies[i];

					// Collision with other enemies
					for (int j = 0; j < state.enemies.size(); j++) {
						if (i == j) continue;
						auto& b = state.enemies[j];

						if (aabb(a.pos.x - RADIUS, a.pos.y - RADIUS, RADIUS * 2.0f, RADIUS * 2.0f, b.pos.x - RADIUS, b.pos.y - RADIUS, RADIUS * 2.0f, RADIUS * 2.0f)) {
							Vector2f awayFromB = a.pos - b.pos;
							a.vel = a.vel + awayFromB * 0.5f;
						}
					}
				}

				// enemy health and death
				for (std::vector<Enemy>::iterator mit = state.enemies.begin(); mit != state.enemies.end(); ) {
					auto& enemy = *mit;
					if (enemy.health <= 0.0f) {
						// Add coins and nourishment 
						// TODO: Constant??
						auto coinsAdd = std::uniform_int_distribution(1, 6)(rng);
						auto nourishAdd = std::uniform_real_distribution(0.0f, 5.0f)(rng);
						state.player.nourishment = std::max(std::min(state.player.nourishment + nourishAdd, state.player.maxNourishment), 0.0f);
						state.player.coins += coinsAdd;

						// Play cash effect
						sound.cash.setVolume(100.0f);
						sound.cash.play();

						// Delete enemy
						mit = state.enemies.erase(mit);
					} else {
						mit++;
					}
				}
















				

				// Store stuff (hover on item, buy items)
				store.update(window, state);
        		break;
    		case GameStage::Lost:
	        	draw::gameOver(state, drawCtx);

	        	// Restart with enter
	        	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
	        		state = GameState();
	        	}
    			break;
    	}

		// Update camera pos
		// cameraPos = state.player.pos;
		cameraPos = screenCenter;

		// Draw the game
		window.clear(sf::Color(3, 2, 2));

		// Screen size in game units
		auto cameraTransform = sf::Transform()
								   .scale(TILE_SIZE, TILE_SIZE)
								   .translate((-cameraPos + screenCenter).toSFML());

		// used for game world rendering
		sf::RenderStates worldRenderState;
	    worldRenderState.transform = cameraTransform;

	    // Draw initial border
	    sf::RectangleShape gameBorder(screenSize.toSFML());
	    gameBorder.setFillColor(sf::Color::Transparent);
	    gameBorder.setOutlineColor(sf::Color::White);
	    gameBorder.setOutlineThickness(1.0f);
	    window.draw(gameBorder, worldRenderState);

    	// Draw player
    	auto inset = Vector2f(-RADIUS, -RADIUS);
    	sf::CircleShape playerCircle(RADIUS, 20);
    	playerCircle.setFillColor(sf::Color::Green);
    	playerCircle.setPosition((state.player.pos + inset).toSFML());
    	window.draw(playerCircle, worldRenderState);

    	// Draw player direction
	    sf::Vertex line[] = {
	        sf::Vertex(state.player.pos.toSFML(), sf::Color::Green),
	        sf::Vertex((state.player.pos + state.player.getForward()).toSFML(), sf::Color::Red)
	    };
	    window.draw(line, 2, sf::Lines, worldRenderState);

    	// Draw sprites
    	for (auto& enemy: state.enemies) {
	    	auto inset = Vector2f(-enemy.radius, -enemy.radius);
	    	sf::CircleShape circle(enemy.radius, 20);
	    	circle.setFillColor(enemy.color);
	    	circle.setPosition((enemy.pos + inset).toSFML());
			window.draw(circle, worldRenderState);
		}

		// Draw bullets
    	for (auto& bullet: state.bullets) {
		    sf::Vertex line[] = {
		        sf::Vertex((bullet.pos - bullet.vel * 0.01f).toSFML(), sf::Color::Green),
		        sf::Vertex((bullet.pos).toSFML(), sf::Color::Red)
		    };
		    window.draw(line, 2, sf::Lines, worldRenderState);
		}

		fog.notoriety = (state.player.pos - screenCenter).norm() / 2000.0f;
		fog.draw(window, font, time.getElapsedTime().asSeconds());

		// Draw statusbars
		drawStatusBar(window, font, "health", state.player.health, state.player.maxHealth,
					  0.0, 0.0, 140.0, 30.0, sf::Color::Green, sf::Color::Red);
		drawStatusBar(window, font, "not-hunger", state.player.nourishment,
					  state.player.maxNourishment, 150.0, 0.0, 100.0, 32.0,
					  sf::Color::Yellow, sf::Color::Red);
		drawStatusBar(window, font, "fog-awareness", state.player.sanity,
					  state.player.maxSanity, 330.0, 0.0, 100.0, 49.0,
					  sf::Color(100, 100, 100), sf::Color::Red);

		// Reloading
		drawStatusBar(window, font, "time",
					  std::min(state.gunCooldown.getElapsedTime().asSeconds(), state.gunCooldownTime),
					  state.gunCooldownTime, 200.0, 560.0, 30.0, 20.0,
					  sf::Color(100, 100, 100), sf::Color::Red);

		// Draw pet face

		{
			// Make face float around
			float t = time.getElapsedTime().asSeconds();
			float x = 720.0f + 5.0f * std::sin(t + 1.41f);
			float y = 90.0f + 2.0f * std::cos(t * 2.0f);

			Face face(sf::Vector2f(x, y));
			float howsItGoin =
				((state.player.health / state.player.maxHealth) +
				 (state.player.nourishment / state.player.maxNourishment) +
				 (state.player.sanity / state.player.maxSanity)) /
				3.0f;
			face.frown = (howsItGoin * 3.0) - 2.0;
			face.draw(window);
		}

		// Show coins
		{
			std::stringstream ss;
			ss << "Quid: ";
			ss << state.player.coins;
			sf::Text text;
			text.setFont(font);
			text.setString(ss.str());
			text.setPosition(500.0, 0.0);
			text.setCharacterSize(40);
			window.draw(text);
		}

		// Show wave
		{
			std::stringstream ss;
			if (state.inWave) {
				ss << "Wave: ";
				ss << state.wave;
				drawStatusBar(
					window,
					font,
					ss.str().c_str(),
					state.timeTillNext,
					// WAVE_TIME,
					30.0f,
					670, 540,
					100, 60,
					sf::Color::Green,
					sf::Color::Black
				);
			} else {
				ss << "breaktime";
				drawStatusBar(
					window,
					font,
					ss.str().c_str(),
					state.timeTillNext,
					// BREAK_TIME,
					20.0f,
					670, 540,
					130, 60,
					sf::Color::Yellow,
					sf::Color::Black
				);
			}
			// sf::Text text;
			// text.setFont(font);
			// text.setString(ss.str());
			// text.setPosition(670.0, 540.0);
			// text.setCharacterSize(30);
			// window.draw(text);
		}

		// Show store items
		store.render(window, font);

		window.display();
    }

    return 0;
}
