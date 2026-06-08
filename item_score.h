#ifndef ITEM_SCORE_H
#define ITEM_SCORE_H

#include <ncurses.h>
#include <chrono>
#include <random>
#include <vector>

using namespace std;
using namespace std::chrono;

// 현재 프로젝트의 map 크기
const int ITEM_SCORE_SIZE = 21;

// snake.cpp에 있는 전역 map을 가져와서 사용
extern int map[ITEM_SCORE_SIZE][ITEM_SCORE_SIZE];

// map에서 사용하는 블록 번호
const int EMPTY = 0;
const int WALL = 1;
const int IMMUNE_WALL = 2;
const int SNAKE_HEAD = 3;
const int SNAKE_BODY = 4;
const int GATE = 5;
const int GROWTH_ITEM = 6;
const int POISON_ITEM = 7;
const int SPEED_ITEM = 8;   // 추가 아이템: 먹으면 뱀 속도 증가

class ScoreManager {
private:
    int growthCount;     // Growth Item 획득 횟수
    int poisonCount;     // Poison Item 획득 횟수
    int gateCount;       // Gate 사용 횟수
    int speedItemCount;  // Speed Item 획득 횟수
    int maxLength;       // 게임 중 도달한 최대 길이

    // Mission 목표값
    int missionLength;
    int missionGrowth;
    int missionPoison;
    int missionGate;

    // 게임 시작 시간
    steady_clock::time_point startTime;

public:
    ScoreManager();

    // 점수 갱신 함수
    void updateLength(int currentLength);
    void addGrowth();
    void addPoison();
    void addGate();
    void addSpeedItem();

    // 아이템 효과 적용 함수
    void applyItemEffect(int itemType, int& length, int& speed, bool& gameOver);

    // 점수 정보 반환 함수
    int getGrowthCount() const;
    int getPoisonCount() const;
    int getGateCount() const;
    int getSpeedItemCount() const;
    int getMaxLength() const;
    int getElapsedTime() const;

    // 미션 클리어 여부
    bool isMissionClear(int currentLength) const;

    // ncurses로 점수판 출력
    void draw(int startX, int currentLength) const;
};

class ItemManager {
private:
    struct Item {
        int y;                                  // 아이템 y좌표
        int x;                                  // 아이템 x좌표
        int type;                               // 아이템 종류
        bool active;                            // 현재 활성화 여부
        steady_clock::time_point createdTime;   // 생성 시간
    };

    static const int MAX_ITEMS = 3;             // 동시에 최대 3개
    Item items[MAX_ITEMS];

    milliseconds itemLifeTime;                  // 아이템 유지 시간
    mt19937 rng;                                // 랜덤 생성기

    int countActiveItems() const;
    bool isEmptyCell(int y, int x) const;
    int getRandomItemType();
    bool spawnOneItem();
    void removeExpiredItems();

public:
    ItemManager();

    // 게임 루프에서 계속 호출
    void update();

    // 특정 위치에 있는 아이템 종류 반환
    int getItemTypeAt(int y, int x) const;

    // 특정 위치의 아이템을 비활성화
    void removeItemAt(int y, int x);

    // 해당 위치가 아이템 칸인지 확인
    bool isItemCell(int y, int x) const;
};

#endif