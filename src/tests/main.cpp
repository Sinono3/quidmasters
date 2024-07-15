#include "../GameDef.hpp"
#include "../math/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cmath>

struct PlayerAnimationState {
	Vector2f velocity;
	float angle;
	// bool aiming;
};

int main() {
	sf::RenderWindow window(sf::VideoMode(GameDef::SCREEN_WIDTH, GameDef::SCREEN_HEIGHT), "Quidmasters: Testbench");
	window.setVerticalSyncEnabled(true);

	PlayerAnimationState player;
	sf::Clock clock;
	sf::Clock deltaClock;

	while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float time = clock.getElapsedTime().asSeconds();
		float dt = deltaClock.restart().asSeconds();

		Vector2f screenCenter = Vector2f(0.0f, 0.0f);
    	Vector2f targetVel;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	        targetVel.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	        targetVel.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	        targetVel.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	        targetVel.x += 1;

	    if (targetVel.norm_sq() > 0.0f)
		    targetVel = targetVel.normalized();

		auto mousePos = Vector2(sf::Mouse::getPosition(window)).to<float>();
		auto playerToMouse = (mousePos - sf::Vector2f(400.0f, 300.0f));
		playerToMouse.y *= -1.0f;
		player.angle  = playerToMouse.angle();
		// std::cout << playerToMouse.x << ", " << playerToMouse.y << "\tAngle: " << player.angle << std::endl;
		Vector2f angleVec = Vector2f(1.0f, 0.0f).rotate(player.angle);

        window.clear(sf::Color::Black);

        const float SCALE = GameDef::SCALE * 4.0f;

        sf::RenderStates states;
        sf::Transform transform;
        states.transform = transform.translate(400.0f, 300.0f).scale(SCALE, -SCALE);

	    sf::Vertex velLine[2];
	    velLine[0] = sf::Vertex(screenCenter.toSFML(), sf::Color::Red);
	    velLine[1] = sf::Vertex((screenCenter + targetVel * 2.0f).toSFML(), sf::Color::Red);
	    window.draw(velLine, 2, sf::Lines, states);

	    sf::Vertex angleLine[2];
	    angleLine[0] = sf::Vertex(screenCenter.toSFML(), sf::Color::Green);
	    angleLine[1] = sf::Vertex((screenCenter + angleVec * 2.0f).toSFML(), sf::Color::Green);
	    window.draw(angleLine, 2, sf::Lines, states);

	    // Legs
	    const float LEG_RADIUS = 0.4f;
	    const float LEG_SEP = 0.5f;
	    const float LEG_MAX_FWD_DIST = 0.7f;
	    const float LEG_MAX_LAT_DIST = 0.3f;
	    const float LEG_SPEED = 5.0f;

	    Vector2f leg1Pos(0.0f, LEG_SEP);
	    Vector2f leg2Pos(0.0f, -LEG_SEP);

	    // Forward movement
	    auto forwardMovement = (Vector2f(1.0f, 0.0f).rotate(player.angle)).dot(targetVel);
	    leg1Pos.x += forwardMovement * LEG_MAX_FWD_DIST * std::sin(LEG_SPEED * time);
	    leg2Pos.x += forwardMovement * LEG_MAX_FWD_DIST * std::sin(LEG_SPEED * time + 3.14f);

	    // Lateral movement
	    auto lateralMovement = (Vector2f(0.0f, 1.0f).rotate(player.angle)).dot(targetVel);
	    leg1Pos.y += lateralMovement * LEG_MAX_LAT_DIST * std::sin(LEG_SPEED * time);
	    leg2Pos.y += lateralMovement * LEG_MAX_LAT_DIST * std::sin(LEG_SPEED * time + 1.8f);

	    std::cout << "Fwd: " << forwardMovement << "\t Lateral: " << lateralMovement << std::endl;
	    
	    // Rotate them for the player angle
	    leg1Pos = leg1Pos.rotate(player.angle);
	    leg2Pos = leg2Pos.rotate(player.angle);

	    // Set the spheres on center of this point (not on their corner)
	    leg1Pos = leg1Pos - Vector2f(LEG_RADIUS, LEG_RADIUS);
	    leg2Pos = leg2Pos - Vector2f(LEG_RADIUS, LEG_RADIUS);

	    sf::CircleShape leg(LEG_RADIUS, 10);
	    leg.setFillColor(sf::Color::Cyan);
	    leg.setPosition(leg1Pos.toSFML());
	    window.draw(leg, states);
	    leg.setPosition(leg2Pos.toSFML());
	    window.draw(leg, states);

	    // Head
		sf::CircleShape head(0.5f, 20);
		head.setPosition(-0.5f, -0.5f);
		window.draw(head, states);

	    // Player hitbox overlay
	    sf::CircleShape hitbox(1.0f, 20);
		hitbox.setPosition(-1.0f, -1.0f);
		hitbox.setFillColor(sf::Color::Transparent);
		hitbox.setOutlineColor(sf::Color::Yellow);
		hitbox.setOutlineThickness(0.01f);
		window.draw(hitbox, states);
		window.display();
	}

	return 0;
}
