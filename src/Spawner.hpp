#pragma once
#include "System.hpp"
#include <SFML/System.hpp>

struct Spawner : public System {
    float spawnTime = 3.0;
    sf::Clock timer;

    void update(GameState& state) override;
};

