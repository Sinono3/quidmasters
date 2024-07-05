#include "../GameState.hpp"
#include "../systems.hpp"
#include <cmath>

void systems::music(GameState &state, const FrameContext &frame, Assets::Sound &sound) {
	std::array<sf::Music*, 3> songs {{
		&sound.music_Phase1,
		&sound.music_Phase2,
		&sound.music_Phase3,
	}};
	int currentSong = (state.wave / 5);

	constexpr float LERP_SPEED = 5.0f;
	float step = LERP_SPEED * frame.dt;

	for (int i = 0; i < songs.size(); i++) {
		auto& song = *songs[i];

		// Songs should always be playing during the game
		if (song.getStatus() !=  sf::Music::Playing) song.play();

		float target = (i == currentSong) ? 70.0f : 0.0f;
		float volume = song.getVolume();

		if (std::abs(volume - target) > step) {
			if (volume < target) {
				volume += step;
			} else if (volume > target){
				volume -= step;
			}
		} else {
			volume = target;
		}
		
		song.setVolume(volume);
	}
}
