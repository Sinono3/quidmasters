#pragma once

struct GameState;

struct System {
    virtual void update(GameState& state) {}
};

