#include "controller.h"
#include "SDL.h"

void Controller::handleInput(State &state, InputEntity &entity) const {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            state = State::QUIT;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    entity.handleInput(Input::kUp);
                    break;
                case SDLK_DOWN:
                    entity.handleInput(Input::kDown);
                    break;
                case SDLK_LEFT:
                    entity.handleInput(Input::kLeft);
                    break;
                case SDLK_RIGHT:
                    entity.handleInput(Input::kRight);
                    break;
                case SDLK_RETURN:
                    entity.handleInput(Input::kEnter);
                    break;
            }
        }
    }
}
