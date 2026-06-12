// snake.h
// Declares the Snake class that owns snake state and movement rules.
#ifndef SNAKE_H
#define SNAKE_H

#include <chrono>
#include "item_score.h"

const int SIZE = 21;

class Snake {
public:
    std::chrono::steady_clock::time_point tick = std::chrono::steady_clock::now();
    int body[362][2]{ {8,6}, {7,6}, {6,6} };
    int speed{ 120 };
    int length{ 3 };
    int max_length{ 361 };
    char dir{ 'R' };
    bool turnLocked{ false };
    bool gameOver{ false };
    bool missionClear{ false };
    const char* gameOverReason{ "" };

    void loadFromMap();
    void to_loc(int& x1, int& y1, const int x2, const int y2) const;
    void move(ScoreManager& score, ItemManager& items);
    bool turn(const int key);
    bool getGate(int& x, int& y);
};

#endif
