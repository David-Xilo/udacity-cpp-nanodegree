#include "score.h"


void Score::reset() {
    score = 0;
    size = 1;
}

int Score::getScore() const {
    return score;
}

int Score::getSize() const {
    return size;
}

void Score::setSize(int newSize) {
    size = newSize;
}

void Score::incrementScore() {
    ++score;
}
