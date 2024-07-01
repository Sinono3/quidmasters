#include "../aabb.hpp"
#include "../systems.hpp"
#include <optional>

void systems::bullets(GameState &state, const FrameContext &frame) {
	for (std::vector<Bullet>::iterator mit = state.bullets.begin();
		 mit != state.bullets.end();) {
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
			// Mark for deletion
			shouldDelete = true;
		}

		// Check if the bullet has strayed too far away (we must delete it)
		// If the bullet is out of this AABB, it is too far away
		if (!aabb(bullet.pos.x, bullet.pos.y, 0.1f, 0.1f, -frame.screenSize.x, -frame.screenSize.y, frame.screenSize.x * 3.0f, frame.screenSize.y * 3.0f)) {
			shouldDelete = true;
		}

		if (shouldDelete) {
			// Remove bullet from vector
			mit = state.bullets.erase(mit);
		} else {
			mit++;
		}
	}

	std::cout << state.bullets.size() << std::endl;
}
