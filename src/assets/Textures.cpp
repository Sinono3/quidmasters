#include "../Assets.hpp"
#include <iostream>

Assets::Textures::Textures() {
	if (!bgL1.loadFromFile("sprites/level1.jpeg")
		|| !fog.loadFromFile("sprites/fog2.png")
		|| !icons.pistol.loadFromFile("sprites/guns/pistol.png")
		|| !icons.revolver.loadFromFile("sprites/guns/revolver.png")
		|| !icons.machineGun.loadFromFile("sprites/guns/machinegun.png")
		|| !icons.shotgun.loadFromFile("sprites/guns/shotgun.png")
		|| !icons.bazooka.loadFromFile("sprites/guns/bazooka.png")
		|| !icons.grenade.loadFromFile("sprites/guns/grenade.png")
		|| !icons.medkit.loadFromFile("sprites/guns/grenade.png")
		|| !icons.book.loadFromFile("sprites/guns/grenade.png")
	) {
		std::cerr << "Error while loading textures" << std::endl;
		exit(1);
	}
}
