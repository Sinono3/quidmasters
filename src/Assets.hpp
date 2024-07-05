#pragma once
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Audio/Music.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

struct Assets {
	struct Sound {
		sf::SoundBuffer machineGunBuffer, handgunBuffer, shotgunBuffer,
			cashBuffer, hitBuffer, coinsBuffer;
		sf::Sound machineGun, handgun, shotgun, cash, hit, coins;

		sf::Music music_Phase1;
		sf::Music music_Phase2;
		sf::Music music_Phase3;

		Sound();
	};

	struct Textures {
		sf::Texture bgL1, fog;

		Textures();
	};

	Sound sound;
	Textures textures;
	sf::Font papyrus;

	Assets();
};

