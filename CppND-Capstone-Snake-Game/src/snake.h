#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <map>
#include "SDL.h"
#include "controller.h"
#include "gameState.h"
#include "gameEntity.h"

class Snake : public InputEntity {
public:

    Snake(int grid_width, int grid_height)
            : grid_width(grid_width),
              grid_height(grid_height),
              head_x(grid_width / 2),
              head_y(grid_height / 2) {}

    void update();

    void GrowBody();

    bool SnakeCell(int x, int y);

    void restartSnake(int grid_width, int grid_height);

    void handleInput(Input input);

    float speed{0.1f};
    int size{1};
    bool alive{true};
    float head_x;
    float head_y;
    std::vector<SDL_Point> body;

private:

    Input direction = Input::kUp;

    std::map<Input, Input> opposites{{Input::kUp,    Input::kDown},
                                     {Input::kDown,  Input::kUp},
                                     {Input::kLeft,  Input::kRight},
                                     {Input::kRight, Input::kLeft}};

    void UpdateHead();

    void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

    bool growing{false};
    int grid_width;
    int grid_height;
};

#endif