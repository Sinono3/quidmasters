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

struct Sprite {
	Vector2f pos = {5.0f, 5.0f};
    sf::Texture texture;
};

std::vector<float> zBuffer(SCREEN_WIDTH);

float dist(float ax, float ay, float bx, float by, float angle) {
    return std::sqrt((bx-ax) * (bx-ax) + (by-ay) * (by-ay));
}

void drawRay(sf::RenderWindow& window, Player& player, float rayAngle, float x) {
    float rayX = std::cos(rayAngle);
    float rayY = std::sin(rayAngle);

    float distanceToWall = 0;

    while (distanceToWall < DEPTH) {
        distanceToWall += 0.1f;

        int testX = static_cast<int>(player.pos.x + rayX * distanceToWall);
        int testY = static_cast<int>(player.pos.y + rayY * distanceToWall);

        // If the test point is out of the map, don't check it 
        if (testX < 0 || testX >= MAP_WIDTH || testY < 0 || testY >= MAP_HEIGHT) {
            distanceToWall = DEPTH;
            break;
        } 

        // If the ray collided against a map wall, break the loop
        if (MAP[testY * MAP_WIDTH + testX] == 1) {
            break;
        }
    }
    zBuffer[x] = distanceToWall;

    int ceiling = static_cast<int>((SCREEN_HEIGHT / 2.0) - SCREEN_HEIGHT / distanceToWall);
    int floor = SCREEN_HEIGHT - ceiling;

    float lightingLevel = (1.0f - std::min(distanceToWall / DEPTH, 1.0f));
    sf::Color wallColor(
		(int)(lightingLevel * 59),
		(int)(lightingLevel * 37),
		(int)(lightingLevel * 35)
    );

    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(x, ceiling), wallColor),
        sf::Vertex(sf::Vector2f(x, floor), wallColor)
    };

    window.draw(line, 2, sf::Lines);
}

void drawSprite(sf::RenderWindow& window, Player& player, Sprite& sprite) {
    // Translate sprite position to relative to camera
    auto relativeSpritePos = sprite.pos - player.pos;
    // Rotate the basis of the sprite pos to match the player's (right = +x, forward = +y)
    auto viewSpritePos = Vector2f(
		relativeSpritePos.x * cos(player.angle + M_PI_2) + relativeSpritePos.y * sin(player.angle + M_PI_2),
		-relativeSpritePos.x * sin(player.angle + M_PI_2) + relativeSpritePos.y * cos(player.angle + M_PI_2)
	);
	// auto viewSpritePos = player.transformVector(relativeSpritePos);
	// auto viewSpritePos = player.inverseTransformVector(relativeSpritePos);

    // float distToScreen = (float)SCREEN_WIDTH / (2.0f * tan(FOV / 2.0f));
    float distToScreen = relativeSpritePos.norm();
    auto screenPos = Vector2f(
    	(viewSpritePos.x * distToScreen) / (viewSpritePos.y) + ((float)SCREEN_WIDTH / 2.0f),
    	(float)SCREEN_HEIGHT / 2.0f
    );

    std::cout << "X = " << viewSpritePos.x  << "\tY = " << viewSpritePos.y << std::endl;

    float size = 100.0f / distToScreen;
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(size, size));
    shape.setPosition(screenPos.toSFML());
    window.draw(shape);
}

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
			wasd.y = 1.0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			wasd.y = -1.0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			wasd.x = -1.0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			wasd.x = 1.0;

		// Normalize if non-zero
		if (wasd.norm() > 0.0f) wasd = wasd * (1.0f / wasd.norm());

		Vector2f walkVector = sf::Vector2f(
			wasd.y * cos(player.angle) - wasd.x * sin(player.angle),
			wasd.y * sin(player.angle) + wasd.x * cos(player.angle));

		const float WALK_SPEED = 0.08f;
		player.pos = player.pos + walkVector * WALK_SPEED;

        // Mouse look
		Vector2i screenSize = Vector2(window.getSize()).to<int>();
		Vector2i screenCenter = Vector2i(window.getPosition()) + (screenSize.to<float>() * 0.5f).to<int>();
		Vector2i newMousePos = sf::Mouse::getPosition();
		Vector2i mouseDelta = newMousePos - screenCenter;
		sf::Mouse::setPosition(screenCenter.toSFML());
		const float MOUSE_SENSITIVITY = 0.001f;
		player.angle = player.angle + mouseDelta.x * MOUSE_SENSITIVITY;

        window.clear(sf::Color(3, 2, 2));

        for (int x = 0; x < SCREEN_WIDTH; x++) {
			float rayAngle = (player.angle - FOV/2.0f) + ((float)x / (float)SCREEN_WIDTH) * FOV;
            drawRay(window, player, rayAngle, x);
        }

        for (auto& sprite : sprites) {
        	drawSprite(window, player, sprite);
        }


        // Draw map
        const float TILE_SIZE = 10.0f;
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
    	sf::CircleShape circle(5.0f, 20);
    	circle.setFillColor(sf::Color::Red);
    	circle.setPosition(((player.pos - Vector2f(0.5, 0.5)) * TILE_SIZE).toSFML());
    	window.draw(circle);

    	// Draw player direction in map
	    sf::Vertex line[] = {
	        sf::Vertex((player.pos * TILE_SIZE).toSFML(), sf::Color::Red),
	        sf::Vertex(((player.pos + player.getForward()) * TILE_SIZE).toSFML(), sf::Color::Red)
	    };
	    window.draw(line, 2, sf::Lines);

    	// Draw sprites in map
    	for (auto& sprite: sprites) {
	    	sf::CircleShape circle(5.0f, 20);
	    	circle.setFillColor(sf::Color::Blue);
	    	circle.setPosition(((sprite.pos - Vector2f(0.5, 0.5)) * TILE_SIZE).toSFML());
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
