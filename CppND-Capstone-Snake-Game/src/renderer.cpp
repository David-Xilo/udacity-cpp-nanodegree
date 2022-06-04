#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
        : screen_width(screen_width),
          screen_height(screen_height),
          grid_width(grid_width),
          grid_height(grid_height) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create Window
    sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, screen_width,
                                  screen_height, SDL_WINDOW_SHOWN);

    if (nullptr == sdl_window) {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
    }

    // Create renderer
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == sdl_renderer) {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }

    TTF_Init();
}

Renderer::~Renderer() {
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    SDL_DestroyTexture(titleTexture);
    SDL_DestroyTexture(optionsTexture);
    TTF_Quit();
}

void Renderer::UpdateWindowTitle(int score, int fps) {
    std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
    SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::renderSnakeGame(Snake const &snake, SDL_Point const &food) {
    SDL_Rect block;
    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;

    clearScreen();

    // Render food
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);

    // Render snake's body
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (SDL_Point const &point: snake.body) {
        block.x = point.x * block.w;
        block.y = point.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

    // Render snake's head
    block.x = static_cast<int>(snake.head_x) * block.w;
    block.y = static_cast<int>(snake.head_y) * block.h;
    if (snake.alive) {
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
    } else {
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    }
    SDL_RenderFillRect(sdl_renderer, &block);

    // update Screen
    SDL_RenderPresent(sdl_renderer);
}

void Renderer::clearScreen() {
    // Clear screen
    SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer);
}

// https://gigi.nullneuron.net/gigilabs/displaying-text-in-sdl2-with-sdl_ttf/
void Renderer::renderMenu(const std::string &menuText, MenuChoice &choice, Score &score) {
    SDL_Rect block;
    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;

    clearScreen();

    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    font = TTF_OpenFont(font_path.c_str(), 24);

    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdl_renderer);

    int x = 0;
    int y = 0;

    SDL_Rect titleRect;
    getTextAndRect(x, 0, menuText.c_str(), &titleTexture, &titleRect);
    SDL_RenderCopy(sdl_renderer, titleTexture, NULL, &titleRect);

    y += titleRect.y + titleRect.h;


    if (choice.isPrintScore()) {
        SDL_Rect scText;
        getTextAndRect(x, y, MenuConstants::scoreText.c_str(), &optionsTexture, &scText);
        SDL_RenderCopy(sdl_renderer, optionsTexture, NULL, &scText);
        x += scText.x + scText.w;
        SDL_Rect sc;
        std::string scValue = std::to_string(score.getScore());
        getTextAndRect(x, y, scValue.c_str(), &optionsTexture, &sc);
        SDL_RenderCopy(sdl_renderer, optionsTexture, NULL, &sc);
        y = sc.y + sc.h;

        x = 0;

        SDL_Rect szText;
        getTextAndRect(x, y, MenuConstants::sizeText.c_str(), &optionsTexture, &szText);
        SDL_RenderCopy(sdl_renderer, optionsTexture, NULL, &szText);
        x += szText.x + szText.w;

        SDL_Rect sz;
        std::string szValue = std::to_string(score.getSize());
        getTextAndRect(x, y, szValue.c_str(), &optionsTexture, &sz);
        SDL_RenderCopy(sdl_renderer, optionsTexture, NULL, &sz);
        y = sz.y + sz.h;
    }

    for (auto opt : choice.getOptions()) {
        x = 0;
        if (opt == choice.getChosenOption()) {
            SDL_Rect select;
            getTextAndRect(x, y, MenuConstants::selector.c_str(), &optionsTexture, &select);
            SDL_RenderCopy(sdl_renderer, optionsTexture, NULL, &select);
            x += select.x + select.w;
        }

        SDL_Rect optionRect;
        getTextAndRect(x, y, MenuConstants::optionMap.at(opt).c_str(), &optionsTexture, &optionRect);
        y = optionRect.y + optionRect.h;
        SDL_RenderCopy(sdl_renderer, optionsTexture, NULL, &optionRect);
    }

    SDL_RenderPresent(sdl_renderer);
}

void Renderer::getTextAndRect(int x, int y, const char *text, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

