#include "../math/physics.hpp"
#include "../systems.hpp"
#include <optional>

void systems::bullets::physics(GameState &state, const FrameContext &frame, Assets::Sound& sound, std::vector<Bullet>& bulletVector) {
	for (std::vector<Bullet>::iterator mit = bulletVector.begin();
		 mit != bulletVector.end();) {
		Bullet &bullet = *mit;
		// Apply velocity to bullet
		bullet.pos = bullet.pos + bullet.vel * frame.dt;

		bool shouldDelete = false;

		// Check if bullet collided with enemy
		Enemy *collidedWith = nullptr;

		for (auto &enemy : state.enemies) {
			if (aabb(bullet.pos.x, bullet.pos.y, 0.1f, 0.1f,
					 enemy.pos.x - enemy.radius, enemy.pos.y - enemy.radius,
					 enemy.radius * 2.0f, enemy.radius * 2.0f)) {
				collidedWith = &enemy;
				break;
			}
		}

		if (collidedWith != nullptr) {
			Enemy &enemy = *collidedWith;
			enemy.vel = enemy.vel + bullet.vel * bullet.knockback;
			enemy.health -= bullet.damage;
			sound.hit.play();
			// Mark for deletion
			shouldDelete = true;
		}

		// Check if the bullet has strayed too far away (we must delete it)
		// If the bullet is out of this AABB, it is too far away
		if (!aabb(bullet.pos.x, bullet.pos.y, 0.1f, 0.1f, frame.minX, frame.minY, frame.maxX - frame.minX, frame.maxY - frame.minY)) {
			shouldDelete = true;
		}

		if (shouldDelete) {
			// Remove bullet from vector
			mit = bulletVector.erase(mit);
		} else {
			mit++;
		}
	}
}

void systems::bullets::homing(GameState &state, const FrameContext &frame) {
	// How much force does each grav enemy add
	constexpr float FORCE = 5000.0f;

	// Row = x, Col = y
	int rows = (int)frame.screenSize.x;
	int cols = (int)frame.screenSize.y;
	Vector2f forceField[rows][cols];


	// Calculate force field
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			forceField[i][j] = Vector2f(0.0f, 0.0f);
			Vector2f pos((float)i + 0.5f, (float)j + 0.5f);

			for (auto& enemy : state.enemies) {
				// Only use grav enemies for forcefield
				// if (enemy.kind == Enemy::Kind::Grav) {
					Vector2f force = enemy.pos - pos;
					// Inverse squared gravitational field
					float distSquared = force.dot(force);
					forceField[i][j] = forceField[i][j] + force * (FORCE / distSquared);
				// }
			}
		}
	}

	// Apply force field
	for (auto& bullet : state.homingBullets) {
		int i = (int)bullet.pos.x, j = (int)bullet.pos.y;

		if (i >= 0 && i < rows && j >= 0 && j < cols)
			bullet.vel = bullet.vel + forceField[i][j] * frame.dt;
 	}
}
