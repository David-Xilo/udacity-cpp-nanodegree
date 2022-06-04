#ifndef SDL2TEST_MENUFLOW_H
#define SDL2TEST_MENUFLOW_H

#include <memory>
#include "game.h"
#include "menu.h"
#include "score.h"


class MenuFlow {
public:
    MenuFlow(std::size_t grid_width, std::size_t grid_height);

    void run(std::size_t target_frame_duration);

private:
    std::shared_ptr<State> state = std::make_shared<State>(State::PRE_MENU);
    std::unique_ptr<Game> game;
    std::map<State, std::unique_ptr<Menu>> menus;
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(RendererConstants::kScreenWidth,
                                                                    RendererConstants::kScreenHeight,
                                                                    RendererConstants::kGridWidth,
                                                                    RendererConstants::kGridHeight);
    std::shared_ptr<Score> score = std::make_shared<Score>();
};

#endif //SDL2TEST_MENUFLOW_H
