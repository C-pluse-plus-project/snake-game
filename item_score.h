// item_score.h
// Declares score, mission, and item manager classes plus map cell constants.
#ifndef ITEM_SCORE_H
#define ITEM_SCORE_H

#include "cell_types.h"

#include <chrono>
#include <ncurses.h>
#include <random>

class Board;

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
    bool isEmptyCell(const Board& board, const int y, const int x) const;
    int getRandomItemType();
    bool spawnOneItem(Board& board);
    void removeExpiredItems(Board& board);

public:
    ItemManager();

    void update(Board& board);
    int getItemTypeAt(const int y, const int x) const;
    void removeItemAt(Board& board, const int y, const int x);
    bool isItemCell(const int y, const int x) const;
};

#endif
