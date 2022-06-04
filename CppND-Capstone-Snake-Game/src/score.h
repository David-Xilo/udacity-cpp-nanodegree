#ifndef SDL2TEST_SCORE_H
#define SDL2TEST_SCORE_H

class Score {
public:

    void reset();
    int getScore() const;
    int getSize() const;
    void setSize(int newSize);
    void incrementScore();



private:
    int score{0};
    int size{1};
};

#endif //SDL2TEST_SCORE_H
