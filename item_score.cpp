// item_score.cpp
// Implements score tracking, mission checks, and item spawning/lifetime logic.
#include "item_score.h"

#include "board.h"
#include "food.h"
#include "poison.h"

#include <utility>
#include <vector>

ScoreManager::ScoreManager() {
    growthCount = 0;
    poisonCount = 0;
    gateCount = 0;
    speedItemCount = 0;
    maxLength = 3;

    missionLength = 10;
    missionGrowth = 5;
    missionPoison = 2;
    missionGate = 1;

    startTime = std::chrono::steady_clock::now();
}

void ScoreManager::updateLength(const int currentLength) {
    if (currentLength > maxLength) {
        maxLength = currentLength;
    }
}

void ScoreManager::addGrowth() {
    growthCount++;
}

void ScoreManager::addPoison() {
    poisonCount++;
}

void ScoreManager::addGate() {
    gateCount++;
}

void ScoreManager::addSpeedItem() {
    speedItemCount++;
}

void ScoreManager::applyItemEffect(const int itemType, int& length, int& speed, bool& gameOver) {
    if (itemType == GROWTH_ITEM) {
        length += Food::lengthDelta();
        addGrowth();
    }
    else if (itemType == POISON_ITEM) {
        length += Poison::lengthDelta();
        addPoison();

        if (Poison::isFatalLength(length)) {
            gameOver = true;
        }
    }
    else if (itemType == SPEED_ITEM) {
        if (speed > 40) {
            speed -= 10;
        }

        addSpeedItem();
    }

    updateLength(length);
}

int ScoreManager::getGrowthCount() const {
    return growthCount;
}

int ScoreManager::getPoisonCount() const {
    return poisonCount;
}

int ScoreManager::getGateCount() const {
    return gateCount;
}

int ScoreManager::getSpeedItemCount() const {
    return speedItemCount;
}

int ScoreManager::getMaxLength() const {
    return maxLength;
}

int ScoreManager::getElapsedTime() const {
    return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now() - startTime).count());
}

bool ScoreManager::isMissionClear(const int currentLength) const {
    return currentLength >= missionLength &&
           growthCount >= missionGrowth &&
           poisonCount >= missionPoison &&
           gateCount >= missionGate;
}

void ScoreManager::draw(const int startX, const int currentLength) const {
    mvprintw(1, startX, "Score Board");
    mvprintw(3, startX, "B: %d / %d", currentLength, maxLength);
    mvprintw(4, startX, "+: %d", growthCount);
    mvprintw(5, startX, "-: %d", poisonCount);
    mvprintw(6, startX, "G: %d", gateCount);
    mvprintw(7, startX, "S: %d", speedItemCount);
    mvprintw(8, startX, "Time: %ds", getElapsedTime());

    mvprintw(10, startX, "Mission");
    mvprintw(12, startX, "B: %d %s",
             missionLength,
             currentLength >= missionLength ? "(v)" : "( )");
    mvprintw(13, startX, "+: %d %s",
             missionGrowth,
             growthCount >= missionGrowth ? "(v)" : "( )");
    mvprintw(14, startX, "-: %d %s",
             missionPoison,
             poisonCount >= missionPoison ? "(v)" : "( )");
    mvprintw(15, startX, "G: %d %s",
             missionGate,
             gateCount >= missionGate ? "(v)" : "( )");
}

ItemManager::ItemManager()
    : itemLifeTime(5000), rng(std::random_device{}()) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].y = 0;
        items[i].x = 0;
        items[i].type = EMPTY;
        items[i].active = false;
    }
}

int ItemManager::countActiveItems() const {
    int count = 0;

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active) {
            count++;
        }
    }

    return count;
}

bool ItemManager::isEmptyCell(const Board& board, const int y, const int x) const {
    return board.getCell(y, x) == EMPTY;
}

int ItemManager::getRandomItemType() {
    std::uniform_int_distribution<int> dist(1, 100);
    const int value = dist(rng);

    if (value <= Food::spawnWeight()) {
        return Food::type();
    }
    if (value <= Food::spawnWeight() + Poison::spawnWeight()) {
        return Poison::type();
    }
    return SPEED_ITEM;
}

bool ItemManager::spawnOneItem(Board& board) {
    std::vector<std::pair<int, int>> emptyCells;

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (isEmptyCell(board, y, x)) {
                emptyCells.push_back({ y, x });
            }
        }
    }

    if (emptyCells.empty()) {
        return false;
    }

    std::uniform_int_distribution<int> posDist(0, static_cast<int>(emptyCells.size()) - 1);
    const int index = posDist(rng);
    const int y = emptyCells[index].first;
    const int x = emptyCells[index].second;
    const int itemType = getRandomItemType();

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].active) {
            items[i].y = y;
            items[i].x = x;
            items[i].type = itemType;
            items[i].active = true;
            items[i].createdTime = std::chrono::steady_clock::now();
            board.setCell(y, x, itemType);

            return true;
        }
    }

    return false;
}

void ItemManager::removeExpiredItems(Board& board) {
    const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].active) {
            continue;
        }

        const long long elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - items[i].createdTime).count();

        if (elapsed >= itemLifeTime.count()) {
            const int y = items[i].y;
            const int x = items[i].x;

            if (board.getCell(y, x) == items[i].type) {
                board.setCell(y, x, EMPTY);
            }

            items[i].active = false;
        }
    }
}

void ItemManager::update(Board& board) {
    removeExpiredItems(board);

    while (countActiveItems() < MAX_ITEMS) {
        if (!spawnOneItem(board)) {
            break;
        }
    }
}

int ItemManager::getItemTypeAt(const int y, const int x) const {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active && items[i].y == y && items[i].x == x) {
            return items[i].type;
        }
    }

    return EMPTY;
}

void ItemManager::removeItemAt(Board& board, const int y, const int x) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active && items[i].y == y && items[i].x == x) {
            items[i].active = false;

            if (board.getCell(y, x) == items[i].type) {
                board.setCell(y, x, EMPTY);
            }

            return;
        }
    }
}

bool ItemManager::isItemCell(const int y, const int x) const {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active && items[i].y == y && items[i].x == x) {
            return true;
        }
    }

    return false;
}
