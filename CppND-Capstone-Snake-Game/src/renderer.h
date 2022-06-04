#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "snake.h"
#include "gameState.h"
#include "menuChoice.h"
#include "score.h"

namespace RendererConstants {
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};
}

// https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
class Renderer {
public:
    Renderer(std::size_t screen_width, std::size_t screen_height,
             std::size_t grid_width, std::size_t grid_height);
//    Renderer &operator=(const Renderer &source) = delete;
    ~Renderer();

    void UpdateWindowTitle(int score, int fps);

    void renderSnakeGame(Snake const &snake, SDL_Point const &food);

    void renderMenu(const std::string& menuText, MenuChoice &choice, Score &score);

private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;

    inline static const std::string font_path = "../fonts/OpenSans-Regular.ttf";

    TTF_Font *font;

    SDL_Texture *titleTexture;
    SDL_Texture *optionsTexture;

    const std::size_t screen_width;
    const std::size_t screen_height;
    const std::size_t grid_width;
    const std::size_t grid_height;

    void clearScreen();

//    void renderTest();

    void getTextAndRect(int x, int y, const char *text, SDL_Texture **texture, SDL_Rect *rect);
};

#endif