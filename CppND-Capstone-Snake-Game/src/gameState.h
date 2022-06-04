#ifndef SDL2TEST_GAMESTATE_H
#define SDL2TEST_GAMESTATE_H

#include <string>

enum class State {
    PRE_MENU, GAME, POST_MENU, QUIT
};

enum class Input {
    kUp, kDown, kLeft, kRight, kEnter
};

#endif //SDL2TEST_GAMESTATE_H
