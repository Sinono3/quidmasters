#include "../systems.hpp"
#include "../GameDef.hpp"
#include "../math/physics.hpp"

void systems::enemy::ai(GameState &state, const FrameContext &frame) {
	for (auto &enemy : state.enemies) {
		// Movement
		Vector2f targetVel =
			(state.player.pos - enemy.pos).normalized() * enemy.maxSpeed;
		Vector2f velDiff = targetVel - enemy.vel;
		Vector2f accel = velDiff;
		if (velDiff.norm() > (enemy.acceleration * frame.dt)) {
			accel = velDiff.normalized() * enemy.acceleration;
		} else {
			accel = velDiff;
		}
		enemy.vel = enemy.vel + accel * frame.dt;
		enemy.pos = enemy.pos + enemy.vel * frame.dt;

		// Damage player
		if (circleCollision(state.player.pos, Player::RADIUS, enemy.pos, enemy.radius)) {
			state.player.health -= enemy.damagePerSecond;
		}
	}
}
void systems::enemy::collision(GameState &state, const FrameContext &frame) {
	for (int i = 0; i < state.enemies.size(); i++) {
		auto &a = state.enemies[i];

		// Collision with other enemies
		for (int j = 0; j < state.enemies.size(); j++) {
			if (i == j)
				continue;
			auto &b = state.enemies[j];

			if (circleCollision(a.pos, a.radius, b.pos, b.radius)) {
				Vector2f awayFromB = a.pos - b.pos;
				a.vel = a.vel + awayFromB * 0.5f;
			}
		}

		// Limit coordinates to game world
		a.pos = a.pos.clamp(frame.minX, frame.minY, frame.maxX, frame.maxY);
	}
}
void systems::enemy::death(GameState &state, const FrameContext &frame, Assets::Sound &sound) {
	// enemy health and death
	for (std::vector<Enemy>::iterator mit = state.enemies.begin();
		 mit != state.enemies.end();) {
		auto &enemy = *mit;
		if (enemy.health <= 0.0f) {
			// Nourish the pet
			auto nourishAdd = std::uniform_real_distribution(0.0f, 5.0f)(frame.rng);
			state.player.nourishment =
				std::max(std::min(state.player.nourishment + nourishAdd,
								  state.player.maxNourishment),
						 0.0f);


			// Add a quid drop
			int quidDrop = enemy.calculateRandomQuidDrop(frame.rng);
			state.quidDrops.push_back(QuidDrop { enemy.pos, quidDrop });

			// Delete enemy
			mit = state.enemies.erase(mit);
		} else {
			mit++;
		}
	}
}
