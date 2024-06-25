#include "math/Vector2.hpp"
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

struct Player {
	Vector2f pos = {2.0f, 2.0f};
    float angle = 0.0f;
    float health = 10.0f, maxHealth = 10.0f;

    Vector2f getForward() {
    	return transformVector(Vector2f(1.0f, 0.0f));
	}

    Vector2f transformVector(const Vector2f& vector) {
    	return Vector2f(
	    	vector.x * cos(angle) - vector.y * sin(angle),
	    	vector.x * sin(angle) + vector.y * cos(angle)
	    );
    }

	Vector2f inverseTransformVector(const Vector2f& vector) {
		return Vector2f(
			vector.x * cos(angle) + vector.y * sin(angle),
			-vector.x * sin(angle) + vector.y * cos(angle)
		);
	}
};

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


struct Enemy {
	Vector2f pos = {5.0f, 5.0f};
	Vector2f vel;
    sf::Texture texture;
};

struct Bullet {
	Vector2f pos;
	Vector2f vel;
};

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
	std::vector<Enemy> enemies;
    Enemy enemy;
    enemy.pos.x = 5.5f;
    enemy.pos.y = 5.5f;
    if (!enemy.texture.loadFromFile("sprites/sprite.jpg")) {
        // Handle sprite texture loading error
        return -1;
    }
    enemies.push_back(enemy);

    sf::Clock machineGun;
    float machineGunTime = 0.1f;
    std::vector<Bullet> bullets;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		Vector2f wasd;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
			wasd.y = -1.0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			wasd.y = 1.0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			wasd.x = -1.0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			wasd.x = 1.0;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			if (machineGun.getElapsedTime().asSeconds() > machineGunTime) {
				machineGun.restart();
				machineGunTime = (std::normal_distribution(0.1f, 0.1f))(rng);

				// Fire, fire, fire
				// auto pos = player.pos + player.getForward() * 0.2f;
				auto pos = player.pos;
				auto vel = player.getForward() * 0.8f;
				bullets.push_back(Bullet { pos, vel });
				sound.setBuffer(sfxMG);
				std::normal_distribution n(1.0, 0.01);
				sound.setPitch(n(rng));
				sound.play();
			}
		}

		// Normalize if non-zero
		if (wasd.norm() > 0.0f) wasd = wasd * (1.0f / wasd.norm());

		const float WALK_SPEED = 0.08f;

		Vector2f velocity = wasd * WALK_SPEED;
		player.pos = player.pos + velocity;

        // Mouse look
        const float TILE_SIZE = 20.0f;

		Vector2f mousePos = Vector2i(sf::Mouse::getPosition(window)).to<float>();
		Vector2f gameMousePos = mousePos * (1.0f / TILE_SIZE);
		Vector2f playerToMouse = gameMousePos - player.pos;
		player.angle = atan2(playerToMouse.y, playerToMouse.x);


		for (auto& bullet : bullets) {
			bullet.pos = bullet.pos + bullet.vel;

	    	for (auto& enemy: enemies) {
	    		auto radius = 10.0f;
				if (aabb(bullet.pos.x, bullet.pos.y, 0.1f, 0.1f, enemy.pos.x - 0.5f, enemy.pos.y - 0.5f, 1.0f, 1.0f)) {
					std::cout << "HIT" << std::endl;
					enemy.vel = bullet.vel * 0.1;
				}
			}
		}

    	for (auto& enemy: enemies) {
    		const float ENEMY_MAX_SPEED = 0.12f;
    		const float ENEMY_ACCEL = 0.01f;
    		Vector2f targetVel = (player.pos - enemy.pos).normalized() * ENEMY_MAX_SPEED;
    		Vector2f accel = targetVel - enemy.vel;
    		if (accel.norm() > ENEMY_ACCEL)
				accel = accel.normalized() * ENEMY_ACCEL;
    		enemy.vel = enemy.vel + accel;
    		enemy.pos = enemy.pos + enemy.vel;
		}
	




		

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
		        sf::Vertex(((bullet.pos - bullet.vel) * TILE_SIZE).toSFML(), sf::Color::Green),
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
