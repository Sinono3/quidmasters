#pragma once
#include "SFML/Audio.hpp"
#include <iostream>

struct GameSound {
	sf::SoundBuffer machineGunBuffer, handgunBuffer, shotgunBuffer, cashBuffer;
	sf::Sound machineGun, handgun, shotgun, cash;
	GameSound();
};
