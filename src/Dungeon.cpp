#include "math/Vector2.hpp"
#include "Gun.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

std::default_random_engine rng;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

bool aabb(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	return (x1 < x2 + w2) && (x1 + w1 > x2) && (y1 < y2 + h2) && (y1 + h1 > h2);
}

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
	text.setFillColor(fill);
	text.setOutlineColor(bg);
	if (fraction >= 0.0 && fraction < 0.1) fraction = 0.1;
	if (fraction <= 0.0 && fraction > -0.1) fraction = -0.1;
	float thickness = (1.0 / fraction) * 3.0;

	text.setOutlineThickness(thickness);
	window.draw(text);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "A lonely dungeon");
    window.setVerticalSyncEnabled(true);
    sf::Sound sound;

	sf::Font font;
    sf::SoundBuffer sfxMG;

    if (!sfxMG.loadFromFile("sfx/mg.wav") || !font.loadFromFile("fonts/papyrus.ttf")) {
		std::cerr << "We're fucked" << std::endl;
        return 1;
    }

    Player player;
    Gun gun{
    	// .firePeriod = new UniVar(0.4f, 0.9f),
    	.firePeriod = new UniVar(0.0f, 0.05f),
    	.damage = new UniVar(3.9f, 4.1f),
    	.knockback = new UniVar(0.0f, 0.3f),
    	.bulletSpeed = new UniVar(80.0f, 100.0f),
    };
    // Gun gun{
    // 	.firePeriod = new UniVar(0.08f, 0.1f),
    // 	.damage = new UniVar(3.9f, 4.1f),
    // 	.knockback = new UniVar(0.0f, 0.3f),
    // 	.bulletSpeed = new UniVar(100.0f, 200.0f),
    // };
	EnemyClass zombie{
		.maxSpeed = 20.0f, .acceleration = 180.0f,
		.maxHealth = 10.0f,
		.size = 2.0f
	};

	const float ENEMY_SPAWN_PERIOD = 2.0f;
	std::vector<Enemy> enemies;
	float enemySpawnTime = 0.0f;

	sf::Clock deltaClock;
    sf::Clock machineGun;
    float machineGunTime = 0.1f;
    std::vector<Bullet> bullets;

    while (window.isOpen()) {
    	float dt = deltaClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Player movement
        player.processInput(window, dt);
        // Player guns
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			if (machineGun.getElapsedTime().asSeconds() > machineGunTime) {
				machineGun.restart();

				// Fire, fire, fire
				machineGunTime = gun.firePeriod->get(rng);
				auto damage = gun.damage->get(rng);
				auto knockback = gun.knockback->get(rng);
				auto speed = gun.bulletSpeed->get(rng);

				// Add bullet
				auto pos = player.pos;
				auto vel = player.getForward() * speed;
				bullets.push_back(Bullet{pos, vel, damage, knockback});

				// Play sound effect
				std::normal_distribution n(1.0, 0.01);
				sound.setBuffer(sfxMG);
				sound.setPitch(n(rng));
				sound.setVolume(50.0f);
				sound.play();
			}
		}

		// Bullet collisions
		for (std::vector<Bullet>::iterator mit = bullets.begin(); mit != bullets.end(); ) {
			auto& bullet = *mit;
			bullet.pos = bullet.pos + bullet.vel * dt;

			std::optional<Enemy*> collidedWith;

	    	for (auto& enemy: enemies) {
				if (aabb(bullet.pos.x, bullet.pos.y, 0.1f, 0.1f, enemy.pos.x - 0.5f, enemy.pos.y - 0.5f, 1.0f, 1.0f)) {
					collidedWith = &enemy;
					break;
				}
			}

			if (collidedWith.has_value()) {
				auto& enemy = *collidedWith.value();
				enemy.vel = enemy.vel + bullet.vel * bullet.knockback;
				// Remove bullet from bullets
				mit = bullets.erase(mit);
			} else {
				mit++;
			}
		}

		// Enemy spawning
    	enemySpawnTime += dt;
    	if (enemySpawnTime > ENEMY_SPAWN_PERIOD) {
    		enemySpawnTime = 0.0f;

    		Enemy newEnemy = zombie.produce();
    		newEnemy.pos.x = std::uniform_real_distribution<float>(0.0f, 40.0f)(rng);
    		newEnemy.pos.y = std::uniform_real_distribution<float>(0.0f, 30.0f)(rng);
    		enemies.push_back(newEnemy);
    	}

		// Enemy AI
		for (auto &enemy : enemies) {
    		Vector2f targetVel = (player.pos - enemy.pos).normalized() * enemy.maxSpeed;
    		Vector2f accel = targetVel - enemy.vel;
    		// if (accel.norm() > enemy.acceleration)
				accel = accel.normalized() * enemy.acceleration;
		    enemy.vel = enemy.vel + accel * dt;
    		// enemy.vel = targetVel;
    		enemy.pos = enemy.pos + enemy.vel * dt;
		}




		// Draw the game
		const float TILE_SIZE = 20.0f;
		window.clear(sf::Color(3, 2, 2));

    	// Draw player
    	auto radius = 10.0f;
    	auto inset = Vector2f(-radius, - radius);

    	sf::CircleShape circle(radius, 20);
    	circle.setFillColor(sf::Color::Green);
    	circle.setPosition((player.pos * TILE_SIZE + inset).toSFML());
    	window.draw(circle);

    	// Draw player direction
	    sf::Vertex line[] = {
	        sf::Vertex((player.pos * TILE_SIZE).toSFML(), sf::Color::Green),
	        sf::Vertex(((player.pos + player.getForward()) * TILE_SIZE).toSFML(), sf::Color::Red)
	    };
	    window.draw(line, 2, sf::Lines);

    	// Draw sprites
    	for (auto& enemy: enemies) {
	    	sf::CircleShape circle(radius, 20);
	    	circle.setFillColor(sf::Color::Blue);
	    	circle.setPosition((enemy.pos * TILE_SIZE + inset).toSFML());
			window.draw(circle);
		}

		// Draw bullets
    	for (auto& bullet: bullets) {
	  //   	sf::CircleShape circle(1.0f, 20);
	  //   	circle.setFillColor(sf::Color::Red);
	  //   	circle.setPosition((bullet.pos * TILE_SIZE).toSFML());
			// window.draw(circle);
		    sf::Vertex line[] = {
		        sf::Vertex(((bullet.pos - bullet.vel * 0.01f) * TILE_SIZE).toSFML(), sf::Color::Green),
		        sf::Vertex(((bullet.pos) * TILE_SIZE).toSFML(), sf::Color::Red)
		    };
		    window.draw(line, 2, sf::Lines);
		}

		// Draw statusbars
		drawStatusBar(window, font, "health", player.health, player.maxHealth,
					  0.0, 0.0, 140.0, 30.0, sf::Color::Green, sf::Color::Red);
		// drawStatusBar(window, font, "not-hunger", nourishment,
		// 			  maxNourishment, 150.0, 0.0, 100.0, 32.0,
		// 			  sf::Color::Yellow, sf::Color::Red);
		// drawStatusBar(window, font, "happiness", happiness,
		// 			  maxHappiness, 300.0, 0.0, 100.0, 49.0,
		// 			  sf::Color::Cyan, sf::Color::Red);

		// sf::Text text;
		// text.setFont(font);
		// text.setString("Health");
		// window.draw(text);

		window.display();
    }

    return 0;
}
