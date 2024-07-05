#include "../Assets.hpp"
#include <iostream>

Assets::Textures::Textures() {
	if (!bgL1.loadFromFile("sprites/Level1.jpeg")
		|| !fog.loadFromFile("sprites/fog2.png")
		|| !guns.pistol.loadFromFile("sprites/guns/pistol.png")
		|| !guns.revolver.loadFromFile("sprites/guns/revolver.png")
		|| !guns.machineGun.loadFromFile("sprites/guns/machinegun.png")
		|| !guns.shotgun.loadFromFile("sprites/guns/shotgun.png")
		|| !guns.bazooka.loadFromFile("sprites/guns/bazooka.png")
		|| !guns.grenade.loadFromFile("sprites/guns/grenade.png")
	) {
		std::cerr << "Error while loading textures" << std::endl;
		exit(1);
	}
}
