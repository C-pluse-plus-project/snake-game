// game_manager.h
// Declares GameManager, the top-level controller for screen setup,
// input handling, stage transitions, updates, and rendering.
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "board.h"
#include "item_score.h"
#include "snake.h"

class GameManager {
private:
    Board board;
    Snake snake;
    ItemManager items;
    ScoreManager score;
    bool running;
    bool started;
    int currentStage;
    static const int LAST_STAGE = 4;

    void initScreen();
    void closeScreen();
    void nextStage();
    void handleInput();
    void update();
    void drawInfo() const;
    void render() const;

public:
    GameManager();
    void run();
};

#endif
