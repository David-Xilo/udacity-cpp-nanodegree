#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "gameState.h"
#include "gameEntity.h"

class Controller {
public:
    void handleInput(State &state, InputEntity &entity) const;

private:

};

#endif