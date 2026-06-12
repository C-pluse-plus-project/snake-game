// item_score.h
// Declares score, mission, and item manager classes plus map cell constants.
#ifndef ITEM_SCORE_H
#define ITEM_SCORE_H

#include <chrono>
#include <ncurses.h>
#include <random>

const int ITEM_SCORE_SIZE = 21;
extern int map[ITEM_SCORE_SIZE][ITEM_SCORE_SIZE];

const int EMPTY = 0;
const int WALL = 1;
const int IMMUNE_WALL = 2;
const int SNAKE_HEAD = 3;
const int SNAKE_BODY = 4;
const int GATE = 5;
const int GROWTH_ITEM = 6;
const int POISON_ITEM = 7;
const int SPEED_ITEM = 8;
const int TEMP_WALL = 9;
const int TEMP_WALL_READY = 10;

class ScoreManager {
private:
    int growthCount;
    int poisonCount;
    int gateCount;
    int speedItemCount;
    int maxLength;

    int missionLength;
    int missionGrowth;
    int missionPoison;
    int missionGate;

    std::chrono::steady_clock::time_point startTime;

public:
    ScoreManager();

    void updateLength(const int currentLength);
    void addGrowth();
    void addPoison();
    void addGate();
    void addSpeedItem();

    void applyItemEffect(const int itemType, int& length, int& speed, bool& gameOver);

    int getGrowthCount() const;
    int getPoisonCount() const;
    int getGateCount() const;
    int getSpeedItemCount() const;
    int getMaxLength() const;
    int getElapsedTime() const;

    bool isMissionClear(const int currentLength) const;
    void draw(const int startX, const int currentLength) const;
};

class ItemManager {
private:
    struct Item {
        int y;
        int x;
        int type;
        bool active;
        std::chrono::steady_clock::time_point createdTime;
    };

    static const int MAX_ITEMS = 3;
    Item items[MAX_ITEMS];

    std::chrono::milliseconds itemLifeTime;
    std::mt19937 rng;

    int countActiveItems() const;
    bool isEmptyCell(const int y, const int x) const;
    int getRandomItemType();
    bool spawnOneItem();
    void removeExpiredItems();

public:
    ItemManager();

    void update();
    int getItemTypeAt(const int y, const int x) const;
    void removeItemAt(const int y, const int x);
    bool isItemCell(const int y, const int x) const;
};

#endif
