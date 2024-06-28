#include "../systems.hpp"

const float WAVE_TIME = 30.0f;
const float BREAK_TIME = 20.0f;

const int ENEMY_CLASS_COUNT = 2;
const EnemyClass ENEMY_CLASSES[ENEMY_CLASS_COUNT]{
	// Standard zombie
	{
		.maxSpeed = 10.0f, .acceleration = 180.0f,
		.maxHealth = 10.0f,
		.radius = 1.0f,
		.color = sf::Color(0, 200, 0, 255),
	},
	// big zomba
	{
		.maxSpeed = 5.0f, .acceleration = 180.0f,
		.maxHealth = 40.0f,
		.radius = 2.0f,
		.color = sf::Color(100, 0, 100, 255),
	},
};

void getWaveEnemyClasses(int enemyClassCount[], int wave) {
	// Normal zombie
	enemyClassCount[0] = wave * 10;
	// Big zombie
	enemyClassCount[1] = std::max(0, wave / 2);
}

void waves(GameState &state, const FrameContext &frame) {
	if (state.inWave) {
		if (state.timeTillNext <= 0) {
			state.inWave = false;
			state.timeTillNext = BREAK_TIME;
		}
	} else {
		// In break
		if (state.timeTillNext <= 0) {
			state.inWave = true;
			state.timeTillNext = WAVE_TIME;
			state.wave++;

			// Enemies per wave
			getWaveEnemyClasses(state.enemyClassCount, state.wave);

			for (int cls_idx = 0; cls_idx < ENEMY_CLASS_COUNT; cls_idx++) {
				for (int i = 0; i < state.enemyClassCount[cls_idx]; i++) {
					EnemyClass enemyClass = ENEMY_CLASSES[cls_idx];
					Enemy newEnemy = enemyClass.produce();
					newEnemy.pos.x = std::uniform_real_distribution<float>(
						frame.screenSize.x, frame.screenSize.x + 3.0f)(frame.rng);
					newEnemy.pos.y = std::uniform_real_distribution<float>(
						0.0f, frame.screenSize.y)(frame.rng);
					state.enemies.push_back(newEnemy);
				}
			}
		}
	}

	state.timeTillNext -= frame.dt;
}
