#include "menuflow.h"

MenuFlow::MenuFlow(std::size_t grid_width, std::size_t grid_height) {
    game = std::make_unique<Game>(grid_width, grid_height, state, score);
    menus.insert(std::pair<State, std::unique_ptr<Menu>>(State::PRE_MENU,
                                                         std::make_unique<Menu>(MenuConstants::preMenuTitle,
                                                                                MenuConstants::preMenuOptions, state,
                                                                                score, false)));
    menus.insert(std::pair<State, std::unique_ptr<Menu>>(State::POST_MENU,
                                                         std::make_unique<Menu>(MenuConstants::postMenuTitle,
                                                                                MenuConstants::postMenuOptions, state,
                                                                                score, true)));
}

void MenuFlow::run(std::size_t target_frame_duration) {
    while (*state != State::QUIT) {
        switch (*state) {
            case State::GAME :
                game->run(renderer, target_frame_duration);
                break;
            case State::PRE_MENU :
                menus[State::PRE_MENU]->run(renderer, target_frame_duration);
                break;
            case State::POST_MENU :
                menus[State::POST_MENU]->run(renderer, target_frame_duration);
                break;
            case State::QUIT :
                return;
        }
    }

}

