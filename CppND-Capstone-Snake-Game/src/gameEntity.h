#ifndef SDL2TEST_GAMEENTITY_H
#define SDL2TEST_GAMEENTITY_H

#include <memory>
#include "gameState.h"

class Renderer;

class GameEntity {
public:
    virtual void run(std::unique_ptr<Renderer> &renderer,
             std::size_t target_frame_duration) = 0;
};

class InputEntity {
public:
    virtual void handleInput(Input input) = 0;
};

#endif //SDL2TEST_GAMEENTITY_H
