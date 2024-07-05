#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Gun.hpp"

struct Assets {
	struct Sound {
		sf::SoundBuffer machineGunBuffer, handgunBuffer, shotgunBuffer,
			cashBuffer, hitBuffer, coinsBuffer, menuSelectBuffer;
		sf::Sound machineGun, handgun, shotgun, cash, hit, coins, menuSelect;

		sf::Music heartbeat;

		sf::Music music_Phase1;
		sf::Music music_Phase2;
		sf::Music music_Phase3;

		Sound();
	};

	struct Textures {
		sf::Texture bgL1, fog;
		struct {
			sf::Texture pistol;
			sf::Texture revolver;
			sf::Texture shotgun;
			sf::Texture machineGun;
			sf::Texture bazooka;
			sf::Texture grenade;

			const sf::Texture& get(Gun::Icon icon) const {
				switch(icon) {
					case Gun::Icon::Bazooka: return bazooka; break;
					case Gun::Icon::Grenade: return grenade; break;
					case Gun::Icon::MachineGun: return machineGun; break;
					case Gun::Icon::Pistol: return pistol; break;
					case Gun::Icon::Revolver: return revolver; break;
					case Gun::Icon::Shotgun: return shotgun; break;
				}
			}
		} guns;

		Textures();
	};

	Sound sound;
	Textures textures;
	sf::Font papyrus;

	Assets();
};

