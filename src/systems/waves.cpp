#include "../systems.hpp"
#include "../GameDef.hpp"

void getWaveEnemyClasses(EnemyClassCounter& enemyClassCount, int wave) {
	// Normal zombie
	enemyClassCount[0] = wave * 6;
	// Big zombie
	enemyClassCount[1] = std::max(0, (int)(wave / 1.7f));
	// Fast zombie
	enemyClassCount[2] = std::max(0, wave - 7);
}

void systems::waves(GameState &state, const FrameContext &frame) {
	if (!state.inBreak) {
		// Count of enemies that are spawned in this wave
		int totalEnemyCount = 0;
		// Check if wave is over
		bool noEnemies = state.enemies.size() == 0;
		bool allEnemiesSpawned = true;

		for (int i = 0; i < state.enemyClassCount.size(); i++) {
			totalEnemyCount += state.enemyClassCount[i];
			if (state.spawnedEnemyClassCount[i] != state.enemyClassCount[i]) {
				allEnemiesSpawned = false;
				break;
			}
		}
		// The wave is over once all the enemies have spawned 
		// and there are no longer any enemies alive.
		bool waveOver = noEnemies && allEnemiesSpawned;

		if (waveOver) {
			state.inBreak = true;
			state.breakTime = GameDef::BREAK_TIME;
			return;
		}

		float spawnRadius = 1.2f * frame.screenCenter.norm();

		// Spawn enemies
		state.enemySpawnTime -= frame.dt;
		if (state.enemySpawnTime <= 0.0f) {
			float totalEnemySpawnTime = 5.0f * state.wave;
			state.enemySpawnTime = totalEnemySpawnTime / (float)totalEnemyCount;

			for (int cls_idx = 0; cls_idx < state.enemyClassCount.size(); cls_idx++) {
				if (state.spawnedEnemyClassCount[cls_idx] < state.enemyClassCount[cls_idx]) {
					EnemyClass enemyClass = GameDef::ENEMY_CLASSES[cls_idx];
					Enemy newEnemy = enemyClass.produce();
					float angle = std::uniform_real_distribution<float>(0.0f, 6.28f)(frame.rng);
					newEnemy.pos = frame.screenCenter + Vector2f(spawnRadius, 0.0f).rotate(angle);

					state.enemies.push_back(newEnemy);
					state.spawnedEnemyClassCount[cls_idx]++;				
				}
			}
		}

	} else {
		if (state.breakTime <= 0.0f) {
			state.inBreak = false;
			state.wave++;

			// Set new enemy count for this wave
			getWaveEnemyClasses(state.enemyClassCount, state.wave);
			// Reset spawned enemy counter
			state.spawnedEnemyClassCount.fill(0);
		} else {
			state.breakTime -= frame.dt;
		}
	}
}
