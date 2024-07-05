#pragma once
#include "Assets.hpp"
#include "GameState.hpp"
#include <random>

struct FrameContext {
	// Delta time
	float dt;

	// In world space
	Vector2i screenMousePos;
	Vector2f mousePos;
	Vector2f cameraPos;

	// Random
	std::default_random_engine &rng;
};

namespace systems {
namespace player {
void movement(GameState &state, const FrameContext &frame);
void guns(GameState &state, const FrameContext &frame, Assets::Sound &sound);
void hunger(GameState &state, const FrameContext &frame);
void loseCondition(GameState &state, const FrameContext &frame, Assets::Sound& sound);
void quidPickup(GameState &state, const FrameContext &frame, Assets::Sound& sound);
void warnings(GameState &state, const FrameContext &frame, Assets::Sound& sound);
} // namespace player

namespace bullets {
void physics(GameState &state, const FrameContext &frame, Assets::Sound& sound, std::vector<Bullet>&);
void homing(GameState &state, const FrameContext &frame);
}
void waves(GameState &state, const FrameContext &frame);

namespace enemy {
void ai(GameState &state, const FrameContext &frame);
void collision(GameState &state, const FrameContext &frame);
void death(GameState &state, const FrameContext &frame, Assets::Sound &sound);
}; // namespace enemy

void message(GameState &state, const FrameContext &frame);
void store(GameState &state, const FrameContext &frame);

/// WTFFFFFF HAXX????? !!
void hacks(GameState &state, const FrameContext &frame);
} // namespace systems
