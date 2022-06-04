#include "menu.h"
#include "gameState.h"

#include <utility>

void Menu::run(std::unique_ptr<Renderer> &renderer, std::size_t target_frame_duration) {
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;
    choice.reset();
    renderer->renderMenu(title, choice, *score);

    while ((*state == State::PRE_MENU || *state == State::POST_MENU) && !choice.isChosen()) {
        frame_start = SDL_GetTicks();

        // Input, update, Render - the main GAME loop.
        controller->handleInput(*state, choice);
        update();
        renderer->renderMenu(title, choice, *score);

        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle
        // takes.
        frame_count++;
        frame_duration = frame_end - frame_start;

        // After every second, update the window title.
        if (frame_end - title_timestamp >= 1000) {
//            renderer.UpdateWindowTitle(score, frame_count);
            frame_count = 0;
            title_timestamp = frame_end;
        }

        // If the time for this frame is too small (i.e. frame_duration is
        // smaller than the target ms_per_frame), delay the loop to
        // achieve the correct frame rate.
        if (frame_duration < target_frame_duration) {
            SDL_Delay(target_frame_duration - frame_duration);
        }
    }
}

void Menu::update() {
    if (choice.isChosen()) {
        switch (choice.getChosenOption()) {
            case MenuConstants::Option::REPLAY:
            case MenuConstants::Option::PLAY:
                *state = State::GAME;
                break;
            case MenuConstants::Option::QUIT:
                *state = State::QUIT;
                break;
        }
    }
}

Menu::Menu(std::string titleText, std::vector<MenuConstants::Option> opts, std::shared_ptr<State> state,
           std::shared_ptr<Score> score, bool pScore):
title(std::move(titleText)), choice(std::move(opts), pScore), state(std::move(state)), score(std::move(score)) {
}
