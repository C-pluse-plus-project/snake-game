// snake.cpp
// Implements snake movement, collision checks, direction changes, and stage reset.
#include "snake.h"

#include "gate.h"
#include <ncurses.h>

void Snake::to_loc(int& x1, int& y1, const int x2, const int y2) const {
    if (dir == 'R') {
        x1 = x2 + 1;
        y1 = y2;
    }
    else if (dir == 'L') {
        x1 = x2 - 1;
        y1 = y2;
    }
    else if (dir == 'U') {
        x1 = x2;
        y1 = y2 - 1;
    }
    else if (dir == 'D') {
        x1 = x2;
        y1 = y2 + 1;
    }
}

void Snake::move(ScoreManager& score, ItemManager& items) {
    if (gameOver || missionClear) {
        return;
    }

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - tick).count();
    if (elapsed < speed) {
        return;
    }

    int frontX = 0;
    int frontY = 0;
    to_loc(frontX, frontY, body[0][0], body[0][1]);

    if (frontY < 0 || frontY >= SIZE || frontX < 0 || frontX >= SIZE) {
        gameOverReason = "Out of bounds";
        gameOver = true;
        return;
    }

    int target = map[frontY][frontX];
    if (target == GATE) {
        score.addGate();
        if (!getGate(frontX, frontY)) {
            return;
        }
        target = map[frontY][frontX];
    }

    const bool movingIntoTail =
        frontX == body[length - 1][0] && frontY == body[length - 1][1];

    if (target == WALL || target == IMMUNE_WALL || target == TEMP_WALL) {
        gameOverReason = "Hit a wall";
        gameOver = true;
        return;
    }

    if ((target == SNAKE_BODY && !movingIntoTail) || target == SNAKE_HEAD) {
        gameOverReason = "Hit yourself";
        gameOver = true;
        return;
    }

    const int oldLength = length;
    if (target == GROWTH_ITEM || target == POISON_ITEM || target == SPEED_ITEM) {
        score.applyItemEffect(target, length, speed, gameOver);
        items.removeItemAt(frontY, frontX);
    }

    if (gameOver || length < 3) {
        gameOverReason = "Length dropped below 3";
        gameOver = true;
        return;
    }

    if (length > max_length) {
        missionClear = true;
        return;
    }

    for (int i = 0; i < oldLength; i++) {
        const int x = body[i][0];
        const int y = body[i][1];
        if (map[y][x] == SNAKE_HEAD || map[y][x] == SNAKE_BODY) {
            map[y][x] = EMPTY;
        }
    }

    for (int i = length - 1; i > 0; i--) {
        body[i][0] = body[i - 1][0];
        body[i][1] = body[i - 1][1];
    }
    body[0][0] = frontX;
    body[0][1] = frontY;

    for (int i = 0; i < length; i++) {
        map[body[i][1]][body[i][0]] = (i == 0) ? SNAKE_HEAD : SNAKE_BODY;
    }

    score.updateLength(length);
    missionClear = score.isMissionClear(length);
    turnLocked = false;
    tick = std::chrono::steady_clock::now();
}

bool Snake::turn(const int key) {
    char next = dir;

    if (key == KEY_RIGHT || key == 'd' || key == 'D') next = 'R';
    else if (key == KEY_LEFT || key == 'a' || key == 'A') next = 'L';
    else if (key == KEY_UP || key == 'w' || key == 'W') next = 'U';
    else if (key == KEY_DOWN || key == 's' || key == 'S') next = 'D';
    else return false;

    if (next == dir) {
        return true;
    }

    if ((next == 'R' && dir == 'L') || (next == 'L' && dir == 'R') ||
        (next == 'U' && dir == 'D') || (next == 'D' && dir == 'U')) {
        gameOverReason = "Reversed direction";
        gameOver = true;
        return false;
    }

    if (turnLocked) {
        return false;
    }

    dir = next;
    turnLocked = true;
    return true;
}

bool Snake::getGate(int& x, int& y) {
    const char* gateError = "";
    if (!Gate::moveThrough(x, y, dir, x, y, dir, gateError)) {
        gameOverReason = gateError;
        gameOver = true;
        return false;
    }
    return true;
}

void Snake::loadFromMap() {
    int count = 0;

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (map[y][x] == SNAKE_HEAD) {
                body[0][0] = x;
                body[0][1] = y;
            }
        }
    }

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (map[y][x] == SNAKE_BODY) {
                count++;
                body[count][0] = x;
                body[count][1] = y;
            }
        }
    }

    length = count + 1;
    speed = 120;
    dir = 'L';
    turnLocked = false;
    gameOver = false;
    missionClear = false;
    gameOverReason = "";
    tick = std::chrono::steady_clock::now();
}
