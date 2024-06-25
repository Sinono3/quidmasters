#include "math/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int MAP_WIDTH = 16;
const int MAP_HEIGHT = 16;
const float FOV = 90.0f * M_PI / 180.0f;
const float DEPTH = 16.0f;

// 0 = Empty
// 1 = Wall
// 2 = Enemy
const int MAP[MAP_WIDTH * MAP_HEIGHT] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,
    1,0,0,1,0,0,0,0,0,0,2,0,1,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,
    1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

struct Player {
	Vector2f pos = {2.0f, 2.0f};
    float angle = 0.0f;

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

struct Sprite {
	Vector2f pos = {5.0f, 5.0f};
    sf::Texture texture;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "A lonely dungeon");
    window.setVerticalSyncEnabled(true);

	sf::Font font;
	if (!font.loadFromFile("fonts/papyrus.ttf")) {
		std::cerr << "We're fucked" << std::endl;
		return 1;
	}

    Player player;
	std::vector<Sprite> sprites;
    Sprite enemy;
    enemy.pos.x = 5.5f;
    enemy.pos.y = 5.5f;
    if (!enemy.texture.loadFromFile("sprites/sprite.jpg")) {
        // Handle sprite texture loading error
        return -1;
    }
    sprites.push_back(enemy);

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

		// Normalize if non-zero
		if (wasd.norm() > 0.0f) wasd = wasd * (1.0f / wasd.norm());

		const float WALK_SPEED = 0.08f;
		const int TRIES = 20;

		Vector2f velocity = wasd * WALK_SPEED;
		float speed = velocity.norm();
		Vector2f normalized = velocity.normalized();
		Vector2f newVel = velocity;

		Vector2i coordinate = (player.pos + velocity).to<int>();
		if (MAP[coordinate.y * MAP_WIDTH + coordinate.x] == 1) {
			float curX = player.pos.x + velocity.x;

			for (int x = 0; x < (int)(velocity.x * 100.0f); x++) {
				curX -= velocity.x / 100.0f;

				if (MAP[coordinate.y * MAP_WIDTH + (int)curX] == 0) {
					break;
				}
			}

			float curY = player.pos.y + velocity.y;
			for (int y = 0; y < (int)(velocity.y * 100.0f); y++) {
				curY -= velocity.y / 100.0f;

				if (MAP[(int)curY * MAP_WIDTH + coordinate.x] == 0) {
					break;
				}
			}
			player.pos = Vector2f(curX, curY);
		}

        // Mouse look
        const float TILE_SIZE = 20.0f;

		Vector2f mousePos = Vector2i(sf::Mouse::getPosition(window)).to<float>();
		Vector2f gameMousePos = mousePos * (1.0f / TILE_SIZE);
		Vector2f playerToMouse = gameMousePos - player.pos;
		std::cout << mousePos.x << ", " << mousePos.y << std::endl;
		player.angle = atan2(playerToMouse.y, playerToMouse.x);

        window.clear(sf::Color(3, 2, 2));

        // Draw map
        sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        for (int y = 0; y < MAP_HEIGHT; y++) {
	        for (int x = 0; x < MAP_WIDTH; x++) {
	        	tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);

	        	switch (MAP[y * MAP_WIDTH + x]) {
	        		case 0: tile.setFillColor(sf::Color::Black); break;
	        		case 1: tile.setFillColor(sf::Color::White); break;
        			default: break;
	        	}

	        	window.draw(tile);
	        }
    	}
    	// Draw player in map
    	auto radius = 10.0f;
    	auto inset = Vector2f(-radius, - radius);

    	sf::CircleShape circle(radius, 20);
    	circle.setFillColor(sf::Color::Red);
    	circle.setPosition((player.pos * TILE_SIZE + inset).toSFML());
    	window.draw(circle);

    	// Draw player direction in map
	    sf::Vertex line[] = {
	        sf::Vertex((player.pos * TILE_SIZE).toSFML(), sf::Color::Red),
	        sf::Vertex(((player.pos + player.getForward()) * TILE_SIZE).toSFML(), sf::Color::Red)
	    };
	    window.draw(line, 2, sf::Lines);

    	// Draw sprites in map
    	for (auto& sprite: sprites) {
	    	sf::CircleShape circle(radius, 20);
	    	circle.setFillColor(sf::Color::Blue);
	    	circle.setPosition((sprite.pos * TILE_SIZE + inset).toSFML());
	    	window.draw(circle);
    	}

        // sf::Text text;
        // text.setFont(font);
        // text.setString("Health");
        // window.draw(text);

        window.display();
    }

    return 0;
}
