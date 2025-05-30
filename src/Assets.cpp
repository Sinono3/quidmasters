#include "Assets.hpp"
#include <iostream>

Assets::Assets() {
	if (!papyrus.openFromFile("fonts/papyrus.ttf")) {
		std::cerr << "Failed to load font" << std::endl;
		exit(1);
	}
}
