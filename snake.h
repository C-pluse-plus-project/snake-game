// snake.h
// Declares the Snake class that owns snake state and movement rules.
#ifndef SNAKE_H
#define SNAKE_H

#include "board.h"
#include "item_score.h"

#include <chrono>

class Snake {
private:
    static constexpr int MAX_BODY_CELLS = BOARD_SIZE * BOARD_SIZE;

    std::chrono::steady_clock::time_point tick;
    int body[MAX_BODY_CELLS][2];
    int speed;
    int length;
    int maxLength;
    char dir;
    bool turnLocked;
    bool gameOver;
    bool missionClear;
    const char* gameOverReason;

    void toNextLocation(int& nextX, int& nextY, const int currentX, const int currentY) const;
    bool moveThroughGate(const Board& board, int& x, int& y);

public:
    Snake();

    void loadFromMap(const Board& board);
    void move(Board& board, ScoreManager& score, ItemManager& items);
    bool turn(const int key);

    int getLength() const;
    int getSpeed() const;
    char getDirection() const;
    bool isGameOver() const;
    bool isMissionClear() const;
    const char* getGameOverReason() const;
};

#endif
