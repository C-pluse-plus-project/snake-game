#ifndef SNAKE_LOGIC_H
#define SNAKE_LOGIC_H

#include <chrono>
#include "item_score.h"

using namespace std::chrono;

const int SIZE = 21;

class snake_logic {
public:
    steady_clock::time_point tick = steady_clock::now();
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
    void to_loc(int& x1, int& y1, int x2, int y2) const;
    void move(ScoreManager& score, ItemManager& items);
    bool turn(int key);
    void getGate(int& x, int& y);
};

#endif
