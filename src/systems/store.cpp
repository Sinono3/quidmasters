#include "../GameState.hpp"
#include "../systems.hpp"
#include "../aabb.hpp"

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

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (state.store.hoveredOn.has_value()) {
			int index = state.store.hoveredOn.value();
			auto &item = Store::ITEMS[index];

			if (!state.store.bought[index] &&
				(state.player.coins >= item.price)) {
				state.store.bought[index] = true;
				state.player.coins -= item.price;
				state.guns.push_back(item.gun);
				std::cout << "Bought item!" << std::endl;
			}
		}
	}
}
