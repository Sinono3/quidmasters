#include "../Assets.hpp"

Assets::Sound::Sound() :
	machineGunBuffer("sfx/mg.wav"),
	handgunBuffer("sfx/handgun.wav"),
	shotgunBuffer("sfx/shotgun.wav"),
	cashBuffer("sfx/cash.wav"),
	hitBuffer("sfx/hit.wav"),
	coinsBuffer("sfx/coins.wav"),
	menuSelectBuffer("sfx/menu_select.wav"),
	
	machineGun(machineGunBuffer),
	handgun(handgunBuffer),
	shotgun(shotgunBuffer),
	cash(cashBuffer),
	hit(hitBuffer),
	coins(coinsBuffer),
	menuSelect(menuSelectBuffer),

	heartbeat("sfx/heartbeat.mp3"),
	music_Phase1("music/clapmusic.mp3"),
	music_Phase2("music/1234_lasmanos.mp3"),
	music_Phase3("music/desert3.mp3")
{
	machineGun.setVolume(40.0f);
	shotgun.setVolume(20.0f);
	handgun.setVolume(50.0f);
	coins.setVolume(50.0f);
	hit.setVolume(30.0f);

	heartbeat.setLooping(true);
	music_Phase1.setLooping(true);
	music_Phase1.setVolume(0.0f);
	music_Phase2.setLooping(true);
	music_Phase2.setVolume(0.0f);
	music_Phase3.setLooping(true);
	music_Phase3.setVolume(0.0f);
}
