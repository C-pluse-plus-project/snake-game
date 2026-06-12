// wall.h
// Declares TemporaryWallManager, which makes only board-marked 9 cells blink as temporary walls.
#ifndef WALL_H
#define WALL_H

#include <chrono>
#include <random>
#include <utility>
#include <vector>

class TemporaryWallManager {
private:
    struct TemporaryWall {
        int y;
        int x;
        bool active;
        std::chrono::steady_clock::time_point createdTime;
    };

    static const int MAX_TEMP_WALLS = 4;
    TemporaryWall walls[MAX_TEMP_WALLS];

    std::vector<std::pair<int, int>> spawnPoints;
    std::chrono::milliseconds wallLifeTime;
    std::chrono::milliseconds spawnInterval;
    std::chrono::steady_clock::time_point lastSpawnTime;
    std::mt19937 rng;

    int countActiveWalls() const;
    bool isSpawnablePoint(const int y, const int x) const;
    bool isAlreadyActive(const int y, const int x) const;
    bool spawnOneWall();
    void removeExpiredWalls();
    void restoreInactiveMarks();

public:
    TemporaryWallManager();

    void update();
    void clear();
};

#endif
