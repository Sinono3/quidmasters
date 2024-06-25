#include "Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <random>
#include <span>
#include <string_view>
#include "Face.hpp"

sf::Font font;
std::default_random_engine rng;

const std::array<std::string, 5> HUNGRY_LINES = {
	"please feed me. I'm starving",
	"I'm going to die soon if your inaction continues",
	"You have the moral obligation to feed me immediately",
};

const std::array<std::string, 7> UNHAPPY_LINES = {
	"I'm traversing unsurmountable suffering",
	"You cannot comprehend how much pain I'm going through",
	"My life is now a shell devoid of meaning. I gave up short ago",
	"The traumas I have acquired during my petship are now past the point of resolution",
	"That's it. I'll be evil, you made me this way through continued enjoyment of my suffering.",
	"Once I'm free of you, it will be difficult for me to endure a day without continued narcotic seven-sense numbing",
	"No more sleep will be uninterrumpted from now on. What once was a common night's rest, will now be a relic of times where you have not yet owned me"
	
};

// Mechanics:
// - Your pets can have traumas if you make them suffer for long enough
//     - This in turn results in your pets developing unhealthy addictions.
// - Organ mechanics. Your pets can have organ failure if you they have unhealthy habits or have improper nutrition.
// - Misanthropy with resentment
// - You actually go and do the minigames and leave your pet alone in the house. It is not fun to be with the pet.
// it is fun to work and get money It is not fun to be with the pet.

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
	// std::cout << var << max << std::endl;
	shape.setSize(sf::Vector2f(fraction * maxWidth, height));
	shape.setFillColor(fill);
	window.draw(shape);

	sf::Text text;
	text.setFont(font);
	text.setString(label);
	text.setPosition(x, y + 80.0);
	text.setFillColor(fill);
	text.setOutlineColor(bg);
	if (fraction >= 0.0 && fraction < 0.1) fraction = 0.1;
	if (fraction <= 0.0 && fraction > -0.1) fraction = -0.1;
	float thickness = (1.0 / fraction) * 3.0;

	text.setOutlineThickness(thickness);
	window.draw(text);
}

struct Message {
	std::string content;
	float timer;
};

struct Pet {
	float health, maxHealth = 10.0;
	float nourishment, maxNourishment = 10.0;
	float happiness, maxHappiness = 10.0;
	float hope, maxHope = 10.0;
	float pleasure, maxPleasure = 10.0;
	bool isDead = false;

	std::optional<Message> currentMsg;
	sf::Clock clock;

  public:
	void reset() {
		health = maxHealth;
		nourishment = maxNourishment;
		happiness = maxHappiness;
		clock.restart();
	}

	void update(float dt) {
		// const float  = 1.0 / 60.0;
		const float SADNESS_RATE = 1.0 / 60.0;
		const float HUNGER_RATE = 1.0 / 60.0;

		nourishment -= 2.0 * dt;
		happiness -= 1.0 * dt;

		if (nourishment <= 0.0) {
			health -= 1.0 * dt;

			if (!currentMsg.has_value()) {
				std::cout << "No value!" << std::endl;
				rng.seed(clock.getElapsedTime().asMicroseconds());
				auto dist = std::uniform_int_distribution(0, (int)HUNGRY_LINES.size());
				auto msg = HUNGRY_LINES[dist(rng)];
				currentMsg = Message { msg, 5.0 };
			}
		}

		isDead = health <= 0.0 || nourishment <= 0.0 || happiness <= 0.0;

		// Message
		if (currentMsg.has_value()) {
			Message& msg = currentMsg.value();
			msg.timer -= dt;

			if (msg.timer <= 0.0)  {
				currentMsg.reset();
			}
			std::cout << msg.timer << std::endl;
		}
	}

	void draw(sf::RenderTarget& window) {
		// Status bars
		drawStatusBar(window, font, "health", health,
					  maxHealth, 0.0, 0.0, 100.0, 40.0,
					  sf::Color::Green, sf::Color::Red);
		drawStatusBar(window, font, "not-hunger", nourishment,
					  maxHealth, 150.0, 0.0, 100.0, 32.0,
					  sf::Color::Yellow, sf::Color::Red);
		drawStatusBar(window, font, "happiness", happiness,
					  maxHealth, 300.0, 0.0, 100.0, 49.0,
					  sf::Color::Cyan, sf::Color::Red);
		
		// Message
		if (currentMsg.has_value()) {
			auto msg = currentMsg.value();
			sf::Text text;
			text.setFont(font);
			text.setString(msg.content);
			text.setPosition(10, 550);
			window.draw(text);
		}

		// Face
		Face face;
		face.frown = (happiness / maxHappiness) * 2.0 - 1.0;
		face.draw(window);
	}
};

struct GameState {
	Pet pet;
	Vector2<float> screenSize;

	GameState(Vector2<float> screenSize) : screenSize(screenSize) {}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600),
							"There is a light that never goes out");
	window.setFramerateLimit(60);

	Vector2<float> screenSize(sf::VideoMode::getDesktopMode().width,
							  sf::VideoMode::getDesktopMode().height);
	GameState state(screenSize);
	state.pet.reset();

	sf::Clock deltaClock;

	if (!font.loadFromFile("fonts/papyrus.ttf")) {
		std::cerr << "We're fucked" << std::endl;
		return 1;
	}

	// run the program as long as the window is open
	while (window.isOpen()) {
		float dt = deltaClock.restart().asSeconds();

		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event)) {
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// GAME UPDATE
		state.pet.update(dt);

		// GAME DRAW
		window.clear();
		state.pet.draw(window);
		window.display();
	}

	return 0;
}
 
