#include "../Assets.hpp"
#include <iostream>

Assets::Textures::Textures() {
	if (!bgL1.loadFromFile("sprites/Level1.jpeg")
		|| !fog.loadFromFile("sprites/fog2.png")
	) {
		std::cerr << "Error while loading textures" << std::endl;
		exit(1);
	}
}
