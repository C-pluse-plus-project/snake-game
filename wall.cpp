// wall.cpp
// Implements temporary wall logic.
// Only cells marked as 9 in board.cpp are used as possible temporary wall positions.
#include "wall.h"

#include "board.h"
#include "item_score.h"

#include <algorithm>
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

    // Treat every 9 placed in board.cpp as a spawn point.
    // The inactive/passable state is shown as lowercase t.
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (map[y][x] == TEMP_WALL) {
                spawnPoints.push_back({ y, x });
                map[y][x] = TEMP_WALL_READY;
            }
        }
    }
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

bool TemporaryWallManager::isSpawnablePoint(const int y, const int x) const {
    if (y <= 0 || y >= BOARD_SIZE - 1 || x <= 0 || x >= BOARD_SIZE - 1) {
        return false;
    }

    // A temporary wall can appear only on a saved 9-position that is currently
    // shown as lowercase t. This prevents overwriting snake, items, gates, etc.
    return map[y][x] == TEMP_WALL_READY && !isAlreadyActive(y, x);
}

bool TemporaryWallManager::spawnOneWall() {
    if (spawnPoints.empty()) {
        return false;
    }

    std::vector<std::pair<int, int>> candidates;
    for (const auto& point : spawnPoints) {
        if (isSpawnablePoint(point.first, point.second)) {
            candidates.push_back(point);
        }
    }

    if (candidates.empty()) {
        return false;
    }

    std::uniform_int_distribution<int> posDist(0, (int)candidates.size() - 1);
    const int index = posDist(rng);
    const int y = candidates[index].first;
    const int x = candidates[index].second;

    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        if (!walls[i].active) {
            walls[i].y = y;
            walls[i].x = x;
            walls[i].active = true;
            walls[i].createdTime = std::chrono::steady_clock::now();
            map[y][x] = TEMP_WALL;
            return true;
        }
    }

    return false;
}

void TemporaryWallManager::removeExpiredWalls() {
    const auto now = std::chrono::steady_clock::now();

    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        if (!walls[i].active) {
            continue;
        }

        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - walls[i].createdTime);

        if (elapsed >= wallLifeTime) {
            if (map[walls[i].y][walls[i].x] == TEMP_WALL) {
                map[walls[i].y][walls[i].x] = TEMP_WALL_READY;
            }
            walls[i].active = false;
        }
    }
}


void TemporaryWallManager::restoreInactiveMarks() {
    for (const auto& point : spawnPoints) {
        const int y = point.first;
        const int x = point.second;

        if (!isAlreadyActive(y, x) && map[y][x] == EMPTY) {
            map[y][x] = TEMP_WALL_READY;
        }
    }
}

void TemporaryWallManager::update() {
    removeExpiredWalls();
    restoreInactiveMarks();

    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - lastSpawnTime);

    if (elapsed >= spawnInterval && countActiveWalls() < MAX_TEMP_WALLS) {
        if (spawnOneWall()) {
            lastSpawnTime = now;
        }
        else {
            // Avoid trying every frame when every marked point is occupied.
            lastSpawnTime = now;
        }
    }
}

void TemporaryWallManager::clear() {
    for (int i = 0; i < MAX_TEMP_WALLS; i++) {
        if (walls[i].active && map[walls[i].y][walls[i].x] == TEMP_WALL) {
            map[walls[i].y][walls[i].x] = TEMP_WALL_READY;
        }

        walls[i].active = false;
        walls[i].y = 0;
        walls[i].x = 0;
    }

    lastSpawnTime = std::chrono::steady_clock::now();
}
