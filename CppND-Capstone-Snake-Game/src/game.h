#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "gameEntity.h"
#include "score.h"

class Game : public GameEntity {
public:
    Game(std::size_t grid_width, std::size_t grid_height, std::shared_ptr<State> state, std::shared_ptr<Score> score);

    void run(std::unique_ptr<Renderer> &renderer, std::size_t target_frame_duration) override;

//    void run(Renderer &renderer, std::size_t target_frame_duration);

    void restartGame();

private:
    Snake snake;
    SDL_Point food;

    std::shared_ptr<Score> score;

    std::unique_ptr<Controller> controller = std::make_unique<Controller>();
    std::shared_ptr<State> state;

    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;

    int grid_width;
    int grid_height;

    void PlaceFood();

    void update();
};

#endif