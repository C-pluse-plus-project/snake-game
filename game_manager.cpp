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
    resize_term(30, 70);
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
    snake.loadFromMap();
    items = ItemManager();
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

    if (snake.missionClear) {
        if (currentStage < LAST_STAGE && (key == 'n' || key == 'N')) {
            nextStage();
        }
        return;
    }

    if (snake.gameOver) {
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

    if (snake.gameOver || snake.missionClear) {
        return;
    }

    items.update();
    snake.move(score, items);
}

void GameManager::drawInfo() const {
    const int y = board.getSize();

    mvprintw(y + 3, 1, "Stage: %d / %d", currentStage, LAST_STAGE);
    mvprintw(y + 4, 1, "Arrows/WASD: move | q: quit");
    mvprintw(y + 5, 1, "# Wall  X Immune  G Gate  + Growth  - Poison  S Speed");
    mvprintw(y + 6, 1, "Direction: %c | Speed: %dms", snake.dir, snake.speed);
}

void GameManager::render() const {
    clear();

    board.draw();

    const int scoreBoardX = board.getSize() * 2 + 5;
    score.draw(scoreBoardX, snake.length);
    drawInfo();

    if (snake.gameOver) {
        mvprintw(board.getSize() + 7, 1, "Game Over - press q to exit");
        mvprintw(board.getSize() + 8, 1, "Reason: %s", snake.gameOverReason);
    }
    else if (snake.missionClear) {
        if (currentStage < LAST_STAGE) {
            mvprintw(board.getSize() + 8, 1, "Mission Clear - press n for next stage");
        }
        else {
            mvprintw(board.getSize() + 8, 1, "All Stages Clear - press q to exit");
        }
    }
    else if (!started) {
        mvprintw(board.getSize() + 7, 1, "Press an arrow key or WASD to start");
    }

    refresh();
}

GameManager::GameManager() : running(true), started(false), currentStage(1) {
    snake.loadFromMap();
}

void GameManager::run() {
    initScreen();

    while (running) {
        handleInput();
        update();
        render();
        napms(10);
    }

    closeScreen();
}
