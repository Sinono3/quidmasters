#pragma once
#include <SFML/Graphics.hpp>

constexpr int FOGQ_X = 10;
constexpr int FOGQ_Y = 10;

struct Fog {
 sf::Texture fogTexture;
 // Fog quadrants
 bool fog[FOGQ_Y][FOGQ_X];
 float notoriety = 0.0f;
 std::string fogText;

 Fog();
 void update();
 void draw(sf::RenderWindow& window, sf::Font& font, float t);
};
