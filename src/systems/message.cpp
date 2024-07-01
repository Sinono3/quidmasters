#include "../systems.hpp"

void systems::message(GameState &state, const FrameContext &frame) {
	if (state.message.has_value()) {
		auto& msg = state.message.value();
		msg.time -= frame.dt;

		if (msg.time <= 0.0f) {
			state.message.reset();
		}
	}
}
