#include "game.h"
#include <utility>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, std::shared_ptr<State> st, std::shared_ptr<Score> sc)
        : snake((int)grid_width, (int)grid_height),
          engine(dev()),
          random_w(0, static_cast<int>(grid_width - 1)),
          random_h(0, static_cast<int>(grid_height - 1)),
          grid_width((int)grid_width),
          grid_height((int)grid_height) {
    state = std::move(st);
    score = std::move(sc);
    PlaceFood();
}

void Game::run(std::unique_ptr<Renderer> &renderer, std::size_t target_frame_duration) {
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;

    restartGame();
    while (*state == State::GAME) {
        frame_start = SDL_GetTicks();

        controller->handleInput(*state, snake);
        update();
        renderer->renderSnakeGame(snake, food);

        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle
        // takes.
        frame_count++;
        frame_duration = frame_end - frame_start;

        // After every second, update the window title.
        if (frame_end - title_timestamp >= 1000) {
            renderer->UpdateWindowTitle(score->getScore(), frame_count);
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

void Game::PlaceFood() {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);
        // Check that the location is not occupied by a snake item before placing
        // food.
        if (!snake.SnakeCell(x, y)) {
            food.x = x;
            food.y = y;
            return;
        }
    }
}

void Game::update() {
    if (!snake.alive) {
        score->setSize(snake.size);
        *state = State::POST_MENU;
        return;
    }

    snake.update();

    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);

    // Check if there's food over here
    if (food.x == new_x && food.y == new_y) {
        score->incrementScore();
        PlaceFood();
        // Grow snake and increase speed.
        snake.GrowBody();
        snake.speed += 0.02;
    }
}

void Game::restartGame() {
    snake.restartSnake(grid_width, grid_height);
    score->reset();
    PlaceFood();
}
