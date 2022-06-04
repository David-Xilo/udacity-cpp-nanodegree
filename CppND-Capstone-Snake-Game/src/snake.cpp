#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::update() {
    SDL_Point prev_cell{
            static_cast<int>(head_x),
            static_cast<int>(
                    head_y)};  // We first capture the head's cell before updating.
    UpdateHead();
    SDL_Point current_cell{
            static_cast<int>(head_x),
            static_cast<int>(head_y)};  // Capture the head's cell after updating.

    // update all of the body vector items if the snake head has moved to a new
    // cell.
    if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
        UpdateBody(current_cell, prev_cell);
    }
}

void Snake::UpdateHead() {
    switch (direction) {
        case Input::kUp:
            head_y -= speed;
            break;

        case Input::kDown:
            head_y += speed;
            break;

        case Input::kLeft:
            head_x -= speed;
            break;

        case Input::kRight:
            head_x += speed;
            break;
    }

    // Wrap the Snake around to the beginning if going off of the screen.
    head_x = fmod(head_x + grid_width, grid_width);
    head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
    // Add previous head location to vector
    body.push_back(prev_head_cell);

    if (!growing) {
        // Remove the tail from the vector.
        body.erase(body.begin());
    } else {
        growing = false;
        size++;
    }

    // Check if the snake has died.
    for (auto const &item: body) {
        if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
            alive = false;
        }
    }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
    if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
        return true;
    }
    for (auto const &item: body) {
        if (x == item.x && y == item.y) {
            return true;
        }
    }
    return false;
}

void Snake::restartSnake(int grid_width, int grid_height) {
    this->grid_width = grid_width;
    this->grid_height = grid_height;
    this->growing = false;
    this->head_x = grid_width / 2;
    this->head_y = grid_height / 2;
    this->size = 1;
    this->alive = true;
    this->body.clear();
    this->speed = 0.1f;
}

void Snake::handleInput(Input input) {
    if (opposites.find(input) == opposites.end()) { // it's not a direction
        return;
    }
    if (direction != opposites[input] || size == 1) direction = input;
}
