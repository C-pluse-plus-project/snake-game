// gate.cpp
// Implements gate placement and exit-direction selection for the Snake game.
#include "gate.h"

#include "board.h"
#include "item_score.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <utility>
#include <vector>

namespace {
std::mt19937& gateRng() {
    static std::mt19937 rng(
        static_cast<unsigned int>(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    return rng;
}

int dxFor(const char dir) {
    if (dir == 'R') return 1;
    if (dir == 'L') return -1;
    return 0;
}

int dyFor(const char dir) {
    if (dir == 'D') return 1;
    if (dir == 'U') return -1;
    return 0;
}

char oppositeOf(const char dir) {
    if (dir == 'R') return 'L';
    if (dir == 'L') return 'R';
    if (dir == 'U') return 'D';
    return 'U';
}

char clockwiseOf(const char dir) {
    if (dir == 'R') return 'D';
    if (dir == 'D') return 'L';
    if (dir == 'L') return 'U';
    return 'R';
}

char counterClockwiseOf(const char dir) {
    if (dir == 'R') return 'U';
    if (dir == 'U') return 'L';
    if (dir == 'L') return 'D';
    return 'R';
}

bool isInside(const int y, const int x) {
    return y >= 0 && y < BOARD_SIZE && x >= 0 && x < BOARD_SIZE;
}

bool isBlockedCell(const int y, const int x) {
    if (!isInside(y, x)) {
        return true;
    }

    const int cell = map[y][x];
    return cell == WALL ||
           cell == IMMUNE_WALL ||
           cell == GATE ||
           cell == SNAKE_HEAD ||
           cell == SNAKE_BODY;
}

bool hasGateExit(const int y, const int x) {
    if (y == 0) return !isBlockedCell(y + 1, x);
    if (y == BOARD_SIZE - 1) return !isBlockedCell(y - 1, x);
    if (x == 0) return !isBlockedCell(y, x + 1);
    if (x == BOARD_SIZE - 1) return !isBlockedCell(y, x - 1);

    return !isBlockedCell(y - 1, x) ||
           !isBlockedCell(y + 1, x) ||
           !isBlockedCell(y, x - 1) ||
           !isBlockedCell(y, x + 1);
}

char borderExitDirection(const int x, const int y) {
    if (y == 0) return 'D';
    if (y == BOARD_SIZE - 1) return 'U';
    if (x == 0) return 'R';
    if (x == BOARD_SIZE - 1) return 'L';
    return '\0';
}
}

void Gate::placePair() {
    std::vector<std::pair<int, int>> wallCells;

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (map[y][x] == WALL && hasGateExit(y, x)) {
                wallCells.push_back({ y, x });
            }
        }
    }

    if (wallCells.size() < 2) {
        return;
    }

    std::shuffle(wallCells.begin(), wallCells.end(), gateRng());
    map[wallCells[0].first][wallCells[0].second] = GATE;
    map[wallCells[1].first][wallCells[1].second] = GATE;
}

bool Gate::moveThrough(const int entryX,
                       const int entryY,
                       const char currentDir,
                       int& exitX,
                       int& exitY,
                       char& exitDir,
                       const char*& errorReason) {
    int gateX = -1;
    int gateY = -1;

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (map[y][x] == GATE && (x != entryX || y != entryY)) {
                gateX = x;
                gateY = y;
            }
        }
    }

    if (gateX == -1 || gateY == -1) {
        errorReason = "Gate pair missing";
        return false;
    }

    char selectedDir = borderExitDirection(gateX, gateY);

    if (selectedDir == '\0') {
        const char candidates[4] = {
            currentDir,
            clockwiseOf(currentDir),
            counterClockwiseOf(currentDir),
            oppositeOf(currentDir)
        };

        for (int i = 0; i < 4; i++) {
            const int nextX = gateX + dxFor(candidates[i]);
            const int nextY = gateY + dyFor(candidates[i]);
            if (!isBlockedCell(nextY, nextX)) {
                selectedDir = candidates[i];
                break;
            }
        }
    }

    if (selectedDir == '\0') {
        errorReason = "Blocked gate exit";
        return false;
    }

    exitDir = selectedDir;
    exitX = gateX + dxFor(exitDir);
    exitY = gateY + dyFor(exitDir);
    return true;
}
