#include "Assets.hpp"
#include <iostream>

Assets::Assets() {
	if (!papyrus.loadFromFile("fonts/papyrus.ttf")) {
		std::cerr << "Failed to load font" << std::endl;
		exit(1);
	}
}
