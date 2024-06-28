#include "../aabb.hpp"
#include "../systems.hpp"
#include <optional>

void bullets(GameState &state, const FrameContext &frame) {
	for (std::vector<Bullet>::iterator mit = state.bullets.begin();
		 mit != state.bullets.end();) {
		Bullet &bullet = *mit;
		bullet.pos = bullet.pos + bullet.vel * frame.dt;

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
			// Remove bullet from bullets
			mit = state.bullets.erase(mit);
		} else {
			mit++;
		}
	}
}
