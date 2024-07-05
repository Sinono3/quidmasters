#pragma once
#include "SFML/Audio.hpp"
#include <iostream>

struct GameSound {
	sf::SoundBuffer machineGunBuffer, handgunBuffer, shotgunBuffer, cashBuffer, hitBuffer, coinsBuffer;
	sf::Sound machineGun, handgun, shotgun, cash, hit, coins;

	sf::Music music_Phase1;
	sf::Music music_Phase2;
	sf::Music music_Phase3;
	
	GameSound();
};
