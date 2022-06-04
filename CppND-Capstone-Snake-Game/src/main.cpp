#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "menuflow.h"

int main() {


    std::cout << "Game Start " << "\n";
    std::cout << "After Render Start " << "\n";
    std::cout << "After Controller " << "\n";
    MenuFlow menuFlow(RendererConstants::kGridWidth, RendererConstants::kGridHeight);
    std::cout << "After MenuFlow " << "\n";
    menuFlow.run(RendererConstants::kMsPerFrame);
    std::cout << "After run " << "\n";
//  Game GAME(kGridWidth, kGridHeight);
//  GAME.run(controller, renderer, kMsPerFrame);

//  std::cout << "Game has terminated successfully!\n";
//  std::cout << "Score: " << GAME.getScore() << "\n";
//  std::cout << "Size: " << GAME.getSize() << "\n";
    return 0;
}