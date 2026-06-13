// game_manager.cpp
// Implements the main game loop controller and ncurses rendering flow.
#include "game_manager.h"

#include <ncurses.h>

void GameManager::initScreen() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    resize_term(32, 90);
}

void GameManager::closeScreen() {
    endwin();
}

void GameManager::nextStage() {
    if (currentStage >= LAST_STAGE) {
        running = false;
        return;
    }

    currentStage++;
    board.loadMap(currentStage);
    snake.loadFromMap(board);
    items = ItemManager();
    tempWalls = TemporaryWallManager();
    tempWalls.reset(board);
    score = ScoreManager();
    started = false;
    nodelay(stdscr, TRUE);
}

void GameManager::handleInput() {
    const int key = getch();

    if (key == 'q' || key == 'Q') {
        running = false;
        return;
    }

    if (snake.isMissionClear()) {
        if (currentStage < LAST_STAGE && (key == 'n' || key == 'N')) {
            nextStage();
        }
        return;
    }

    if (snake.isGameOver()) {
        return;
    }

    if (snake.turn(key)) {
        started = true;
    }
}

void GameManager::update() {
    if (!started) {
        return;
    }

    if (snake.isGameOver() || snake.isMissionClear()) {
        return;
    }

    items.update(board);
    tempWalls.update(board);
    snake.move(board, score, items);
}

void GameManager::drawInfo() const {
    const int y = board.getSize();

    mvprintw(y + 3, 1, "Stage: %d / %d", currentStage, LAST_STAGE);
    mvprintw(y + 4, 1, "Arrows/WASD: move | q: quit");
    mvprintw(y + 5, 1, "# Wall  X Immune  G Gate  + Growth  - Poison  S Speed");
    mvprintw(y + 6, 1, "T ActiveTempWall  t ReadyTempWall");
    mvprintw(y + 7, 1, "Direction: %c | Speed: %dms",
             snake.getDirection(),
             snake.getSpeed());
}

void GameManager::render() const {
    erase();

    board.draw();

    const int scoreBoardX = board.getSize() * 2 + 5;
    score.draw(scoreBoardX, snake.getLength());
    drawInfo();

    if (snake.isGameOver()) {
        mvprintw(board.getSize() + 8, 1, "Game Over - press q to exit");
        mvprintw(board.getSize() + 9, 1, "Reason: %s", snake.getGameOverReason());
    }
    else if (snake.isMissionClear()) {
        if (currentStage < LAST_STAGE) {
            mvprintw(board.getSize() + 9, 1, "Mission Clear - press n for next stage");
        }
        else {
            mvprintw(board.getSize() + 9, 1, "All Stages Clear - press q to exit");
        }
    }
    else if (!started) {
        mvprintw(board.getSize() + 8, 1, "Press an arrow key or WASD to start");
    }

    refresh();
}

GameManager::GameManager() : running(true), started(false), currentStage(1) {
    snake.loadFromMap(board);
    tempWalls.reset(board);
}

void GameManager::run() {
    initScreen();

    while (running) {
        handleInput();
        update();
        render();
        napms(20);
    }

    closeScreen();
}
