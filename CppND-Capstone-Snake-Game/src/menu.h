#ifndef SDL2TEST_MENU_H
#define SDL2TEST_MENU_H

#include <memory>
#include "controller.h"
#include "renderer.h"
#include "gameState.h"
#include "gameEntity.h"
#include "menuChoice.h"
#include "score.h"

class Menu : public GameEntity {
public:
    Menu(std::string titleText, std::vector<MenuConstants::Option> opts, std::shared_ptr<State> state,
         std::shared_ptr<Score> score, bool pScore);

    void run(std::unique_ptr<Renderer> &renderer, std::size_t target_frame_duration) override;

private:
    std::shared_ptr<Controller> controller = std::make_shared<Controller>();

    std::string title;
    MenuChoice choice;
    std::shared_ptr<State> state;
    std::shared_ptr<Score> score;

    void update();
};


#endif //SDL2TEST_MENU_H
