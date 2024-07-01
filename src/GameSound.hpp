#pragma once
#include "SFML/Audio.hpp"
#include <iostream>

struct GameSound {
	sf::SoundBuffer machineGunBuffer, handgunBuffer, shotgunBuffer, cashBuffer, hitBuffer, coinsBuffer;
	sf::Sound machineGun, handgun, shotgun, cash, hit, coins;
	GameSound();
};
