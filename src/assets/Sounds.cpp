#include <iostream>
#include "../Assets.hpp"

Assets::Sound::Sound() {
	if (!machineGunBuffer.loadFromFile("sfx/mg.wav")
		|| !handgunBuffer.loadFromFile("sfx/handgun.wav")
		|| !shotgunBuffer.loadFromFile("sfx/shotgun.wav")
		|| !cashBuffer.loadFromFile("sfx/cash.wav") 
		|| !coinsBuffer.loadFromFile("sfx/coins.wav") 
		|| !hitBuffer.loadFromFile("sfx/hit.wav")
		|| !music_Phase1.openFromFile("music/clapmusic.mp3")
		|| !music_Phase2.openFromFile("music/1234_lasmanos.mp3")
		// || music_Phase3.openFromFile("music/desert3.mp3")
	) {
		std::cerr << "Error while loading sound assets" << std::endl;
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

	music_Phase1.setLoop(true);
	music_Phase2.setLoop(true);
	music_Phase3.setLoop(true);
}
