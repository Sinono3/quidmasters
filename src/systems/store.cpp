#include "../GameState.hpp"
#include "../systems.hpp"
#include "../math/aabb.hpp"

bool lastFrame = false;

void systems::store(GameState &state, const FrameContext& frame) {
	state.store.hoveredOn.reset();
	for (int i = 0; i < Store::ITEMS.size(); i++) {
		float x = i * Store::ITEM_TILE_SIZE + Store::SLACK;
		float y = SCREEN_HEIGHT - Store::ITEM_TILE_SIZE - Store::SLACK;
		// Check if the mouse is hovering on this item
		if (aabb(x, y, Store::ITEM_TILE_SIZE, Store::ITEM_TILE_SIZE,
				 frame.screenMousePos.x, frame.screenMousePos.y, 0.0f, 0.0f)) {
			state.store.hoveredOn = i;
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !lastFrame) {
		if (state.store.hoveredOn.has_value()) {
			int index = state.store.hoveredOn.value();
			auto &item = Store::ITEMS[index];

			if (!state.store.bought[index]) {
				if (state.player.coins >= item.price) {
					state.store.bought[index] = true;
					state.player.coins -= item.price;
					state.guns.push_back(item.gun);
					state.setMessage(Message(Message::Type::Store, "It's a deal then!", 2.0f));
				} else {
					state.setMessage(Message(Message::Type::Store, "you ain't got enough quid bruh!", 2.0f));
				}
			} else {
				state.setMessage(Message(Message::Type::Store, "you already have that", 2.0f));
			}
		}
	}
	lastFrame = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}
