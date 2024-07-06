#pragma once
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

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
		enum class Icon {
			Bazooka,
			Grenade,
			MachineGun,
			Pistol,
			Revolver,
			Shotgun,

			Medkit,
			Book,
		};

		sf::Texture bgL1, fog;
		struct {
			sf::Texture pistol;
			sf::Texture revolver;
			sf::Texture shotgun;
			sf::Texture machineGun;
			sf::Texture bazooka;
			sf::Texture grenade;
			sf::Texture medkit;
			sf::Texture book;

			const sf::Texture& get(Icon icon) const {
				switch(icon) {
					case Icon::Bazooka: return bazooka; break;
					case Icon::Grenade: return grenade; break;
					case Icon::MachineGun: return machineGun; break;
					case Icon::Pistol: return pistol; break;
					case Icon::Revolver: return revolver; break;
					case Icon::Shotgun: return shotgun; break;
					case Icon::Medkit: return medkit; break;
					case Icon::Book: return book; break;
				}
			}
		} icons;

		Textures();
	};

	Sound sound;
	Textures textures;
	sf::Font papyrus;

	Assets();
};

