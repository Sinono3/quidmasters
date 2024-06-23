#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Vector2.hpp"
#include <random>

struct Guy {
    Vector2<float> position;
};

struct GameState {
    Vector2<float> screenSize;
    std::vector<Guy> guys = {};
    std::default_random_engine rng;

    GameState(Vector2<float> screenSize) : screenSize(screenSize) { }
};

struct System {
    virtual void update(GameState& state) {}
};

struct Spawner : public System {
    float spawnTime = 3.0;
    sf::Clock timer;

    void update(GameState& state) override {
        if (timer.getElapsedTime().asSeconds() > spawnTime) {
            timer.restart();

            const int SLACK = 100;
            std::uniform_int_distribution distX(SLACK, (int)state.screenSize.x - SLACK);
            std::uniform_int_distribution distY(SLACK, (int)state.screenSize.y - SLACK);

            std::cout << "Spawned guy" << std::endl;
            // Spawn guy
            auto guy = Guy { Vector2((float)distX(state.rng), (float)distY(state.rng)) };
            state.guys.push_back(guy);
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "There is a light that never goes out");
    window.setFramerateLimit(60);

    Vector2<float> screenSize(
        sf::VideoMode::getDesktopMode().width,
        sf::VideoMode::getDesktopMode().height
    );
    GameState state(screenSize);
    Spawner spawner;

    // sf::Clock deltaClock;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // sf::Time dt = deltaClock.restart();
        // bird.position.x += 10.0f * dt.asSeconds();

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // GAME UPDATE
        auto cameraPosition = window.getPosition(); // auto fraction = Vector2(position.x / screenSize.x, position.y / screenSize.y); // std::cout << fraction.x << ", " << fraction.y << std::endl;
        spawner.update(state);

        // GAME Draw
        window.clear();
        for (auto guy : state.guys) {
            sf::RectangleShape shape(sf::Vector2f(80.0f, 80.0f));
            shape.setPosition(sf::Vector2f(guy.position.x - cameraPosition.x, guy.position.y - cameraPosition.y));
            window.draw(shape);
        }
        window.display();
    }

    return 0;
}

