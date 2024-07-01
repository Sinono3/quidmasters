#include "GameSound.hpp"

GameSound::GameSound() {
	if (!machineGunBuffer.loadFromFile("sfx/mg.wav")
		|| !handgunBuffer.loadFromFile("sfx/handgun.wav")
		|| !shotgunBuffer.loadFromFile("sfx/shotgun.wav")
		|| !cashBuffer.loadFromFile("sfx/cash.wav") 
		|| !coinsBuffer.loadFromFile("sfx/coins.wav") 
		|| !hitBuffer.loadFromFile("sfx/hit.wav")
	) {
		std::cerr << "We're fucked" << std::endl;
		exit(1);
	}

	machineGun.setBuffer(machineGunBuffer);
	machineGun.setVolume(50.0f);
	shotgun.setBuffer(shotgunBuffer);
	shotgun.setVolume(50.0f);
	handgun.setBuffer(handgunBuffer);
	handgun.setVolume(50.0f);
	cash.setBuffer(cashBuffer);
	coins.setBuffer(coinsBuffer);
	hit.setBuffer(hitBuffer);
	hit.setVolume(30.0f);
}
