// wall.cpp
// Implements temporary wall logic for board-marked spawn positions.
#include "wall.h"

#include "board.h"

#include <utility>
#include <vector>

TemporaryWallManager::TemporaryWallManager()
    : wallLifeTime(2500), spawnInterval(1200),
      lastSpawnTime(std::chrono::steady_clock::now()),
      rng(std::random_device{}()) {
    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        walls[i].y = 0;
        walls[i].x = 0;
        walls[i].active = false;
    }
}

void TemporaryWallManager::reset(Board& board) {
    spawnPoints.clear();

    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        walls[i].y = 0;
        walls[i].x = 0;
        walls[i].active = false;
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (board.getCell(y, x) == TEMP_WALL) {
                spawnPoints.push_back({ y, x });
                board.setCell(y, x, TEMP_WALL_READY);
            }
        }
    }

    lastSpawnTime = std::chrono::steady_clock::now();
}

int TemporaryWallManager::countActiveWalls() const {
    int count = 0;

    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        if (walls[i].active) {
            count++;
        }
    }

    return count;
}

bool TemporaryWallManager::isAlreadyActive(const int y, const int x) const {
    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        if (walls[i].active && walls[i].y == y && walls[i].x == x) {
            return true;
        }
    }

    return false;
}

bool TemporaryWallManager::isSpawnablePoint(const Board& board, const int y, const int x) const {
    if (!board.isInside(y, x) || y == 0 || y == BOARD_SIZE - 1 ||
        x == 0 || x == BOARD_SIZE - 1) {
        return false;
    }

    return board.getCell(y, x) == TEMP_WALL_READY && !isAlreadyActive(y, x);
}

bool TemporaryWallManager::spawnOneWall(Board& board) {
    if (spawnPoints.empty()) {
        return false;
    }

    std::vector<std::pair<int, int>> candidates;
    for (const auto& point : spawnPoints) {
        if (isSpawnablePoint(board, point.first, point.second)) {
            candidates.push_back(point);
        }
    }

    if (candidates.empty()) {
        return false;
    }

    std::uniform_int_distribution<int> posDist(0, static_cast<int>(candidates.size()) - 1);
    const int index = posDist(rng);
    const int y = candidates[index].first;
    const int x = candidates[index].second;

    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        if (!walls[i].active) {
            walls[i].y = y;
            walls[i].x = x;
            walls[i].active = true;
            walls[i].createdTime = std::chrono::steady_clock::now();
            board.setCell(y, x, TEMP_WALL);
            return true;
        }
    }

    return false;
}

void TemporaryWallManager::removeExpiredWalls(Board& board) {
    const auto now = std::chrono::steady_clock::now();

    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        if (!walls[i].active) {
            continue;
        }

        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - walls[i].createdTime);

        if (elapsed >= wallLifeTime) {
            if (board.getCell(walls[i].y, walls[i].x) == TEMP_WALL) {
                board.setCell(walls[i].y, walls[i].x, TEMP_WALL_READY);
            }
            walls[i].active = false;
        }
    }
}

void TemporaryWallManager::restoreInactiveMarks(Board& board) {
    for (const auto& point : spawnPoints) {
        const int y = point.first;
        const int x = point.second;

        if (!isAlreadyActive(y, x) && board.getCell(y, x) == EMPTY) {
            board.setCell(y, x, TEMP_WALL_READY);
        }
    }
}

void TemporaryWallManager::update(Board& board) {
    removeExpiredWalls(board);
    restoreInactiveMarks(board);

    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - lastSpawnTime);

    if (elapsed >= spawnInterval && countActiveWalls() < MAX_TEMP_WALLS) {
        spawnOneWall(board);
        lastSpawnTime = now;
    }
}

void TemporaryWallManager::clear(Board& board) {
    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        if (walls[i].active && board.getCell(walls[i].y, walls[i].x) == TEMP_WALL) {
            board.setCell(walls[i].y, walls[i].x, TEMP_WALL_READY);
        }

        walls[i].active = false;
        walls[i].y = 0;
        walls[i].x = 0;
    }

    lastSpawnTime = std::chrono::steady_clock::now();
}
