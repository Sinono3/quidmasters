#pragma once
#include "GameSound.hpp"
#include "GameState.hpp"
#include <random>


struct FrameContext {
	// Delta time
	float dt;

	// In world space
	Vector2f screenSize;   // Vector (not point)
	Vector2f screenCenter; // Vector (not point)
	Vector2f mousePos;
	Vector2f cameraPos;

	// Random
	std::default_random_engine &rng;
};

namespace player {
void movement(GameState &state, const FrameContext &frame);
void guns(GameState &state, const FrameContext &frame, GameSound &sound);
void hunger(GameState &state, const FrameContext &frame);
void loseCondition(GameState &state, const FrameContext &frame);
} // namespace player

void bullets(GameState &state, const FrameContext &frame);
void waves(GameState &state, const FrameContext &frame);

namespace enemy {
void ai(GameState &state, const FrameContext &frame);
void collision(GameState &state, const FrameContext &frame);
void death(GameState &state, const FrameContext &frame, GameSound &sound);
}; // namespace enemy
