#include "item_score.h"

// =========================
// ScoreManager 구현부
// =========================

ScoreManager::ScoreManager() {
    growthCount = 0;
    poisonCount = 0;
    gateCount = 0;
    speedItemCount = 0;
    maxLength = 3;

    // 기본 Mission 값
    missionLength = 10;
    missionGrowth = 5;
    missionPoison = 2;
    missionGate = 1;

    startTime = steady_clock::now();
}

// 현재 길이가 기존 최대 길이보다 크면 최대 길이 갱신
void ScoreManager::updateLength(int currentLength) {
    if (currentLength > maxLength) {
        maxLength = currentLength;
    }
}

// Growth Item 획득 횟수 증가
void ScoreManager::addGrowth() {
    growthCount++;
}

// Poison Item 획득 횟수 증가
void ScoreManager::addPoison() {
    poisonCount++;
}

// Gate 사용 횟수 증가
void ScoreManager::addGate() {
    gateCount++;
}

// Speed Item 획득 횟수 증가
void ScoreManager::addSpeedItem() {
    speedItemCount++;
}

// 아이템을 먹었을 때 길이, 속도, 점수를 한 번에 처리하는 함수
void ScoreManager::applyItemEffect(int itemType, int& length, int& speed, bool& gameOver) {
    if (itemType == GROWTH_ITEM) {
        // Growth Item: 몸 길이 1 증가
        length++;
        addGrowth();
    }
    else if (itemType == POISON_ITEM) {
        // Poison Item: 몸 길이 1 감소
        length--;
        addPoison();

        // 길이가 3보다 작아지면 Game Over
        if (length < 3) {
            gameOver = true;
        }
    }
    else if (itemType == SPEED_ITEM) {
        // 추가 아이템: 뱀 이동 속도 증가
        // speed 값은 이동 간격(ms)이므로 작아질수록 빨라짐
        if (speed > 40) {
            speed -= 10;
        }

        addSpeedItem();
    }

    // 아이템 효과 적용 후 최대 길이 갱신
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

// 게임 시작 후 지난 시간을 초 단위로 반환
int ScoreManager::getElapsedTime() const {
    return (int)duration_cast<seconds>(steady_clock::now() - startTime).count();
}

// Mission 달성 여부 확인
bool ScoreManager::isMissionClear(int currentLength) const {
    return currentLength >= missionLength &&
           growthCount >= missionGrowth &&
           poisonCount >= missionPoison &&
           gateCount >= missionGate;
}

// Score Board 출력
void ScoreManager::draw(int startX, int currentLength) const {
    mvprintw(1, startX, "Score Board");

    // B: 현재 길이 / 게임 중 최대 길이
    mvprintw(3, startX, "B: %d / %d", currentLength, maxLength);

    // +, -, G 점수
    mvprintw(4, startX, "+: %d", growthCount);
    mvprintw(5, startX, "-: %d", poisonCount);
    mvprintw(6, startX, "G: %d", gateCount);

    // 추가 정보
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

// =========================
// ItemManager 구현부
// =========================

ItemManager::ItemManager()
    : itemLifeTime(5000), rng(random_device{}()) {
    
    // 아이템 슬롯 초기화
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].y = 0;
        items[i].x = 0;
        items[i].type = EMPTY;
        items[i].active = false;
    }
}

// 현재 활성화된 아이템 개수 계산
int ItemManager::countActiveItems() const {
    int count = 0;

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active) {
            count++;
        }
    }

    return count;
}

// 아이템을 생성할 수 있는 빈칸인지 확인
bool ItemManager::isEmptyCell(int y, int x) const {
    return map[y][x] == EMPTY;
}

// 아이템 종류를 랜덤으로 결정
int ItemManager::getRandomItemType() {
    uniform_int_distribution<int> dist(1, 100);
    int value = dist(rng);

    if (value <= 45) {
        return GROWTH_ITEM;      // 45%
    }
    else if (value <= 90) {
        return POISON_ITEM;      // 45%
    }
    else {
        return SPEED_ITEM;       // 10%
    }
}

// 아이템 하나 생성
bool ItemManager::spawnOneItem() {
    vector<pair<int, int>> emptyCells;

    // map 전체를 돌면서 빈칸만 후보로 저장
    for (int y = 0; y < ITEM_SCORE_SIZE; y++) {
        for (int x = 0; x < ITEM_SCORE_SIZE; x++) {
            if (isEmptyCell(y, x)) {
                emptyCells.push_back({ y, x });
            }
        }
    }

    // 빈칸이 없으면 생성 실패
    if (emptyCells.empty()) {
        return false;
    }

    // 빈칸 후보 중 랜덤 위치 선택
    uniform_int_distribution<int> posDist(0, (int)emptyCells.size() - 1);
    int index = posDist(rng);

    int y = emptyCells[index].first;
    int x = emptyCells[index].second;

    // 아이템 종류 랜덤 선택
    int itemType = getRandomItemType();

    // 비어 있는 아이템 슬롯에 등록
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].active) {
            items[i].y = y;
            items[i].x = x;
            items[i].type = itemType;
            items[i].active = true;
            items[i].createdTime = steady_clock::now();

            // map에 아이템 표시
            map[y][x] = itemType;

            return true;
        }
    }

    return false;
}

// 생성 후 5초가 지난 아이템 제거
void ItemManager::removeExpiredItems() {
    steady_clock::time_point now = steady_clock::now();

    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].active) {
            continue;
        }

        long long elapsed =
            duration_cast<milliseconds>(now - items[i].createdTime).count();

        if (elapsed >= itemLifeTime.count()) {
            int y = items[i].y;
            int x = items[i].x;

            // 그 자리에 아직 같은 아이템이 남아 있으면 빈칸으로 변경
            if (map[y][x] == items[i].type) {
                map[y][x] = EMPTY;
            }

            items[i].active = false;
        }
    }
}

// 게임 루프에서 계속 호출하는 함수
void ItemManager::update() {
    // 시간이 지난 아이템 제거
    removeExpiredItems();

    // 활성 아이템이 3개 미만이면 새 아이템 생성
    while (countActiveItems() < MAX_ITEMS) {
        if (!spawnOneItem()) {
            break;
        }
    }
}

// 특정 좌표에 있는 아이템 종류 반환
int ItemManager::getItemTypeAt(int y, int x) const {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active && items[i].y == y && items[i].x == x) {
            return items[i].type;
        }
    }

    return EMPTY;
}

// 특정 좌표의 아이템 제거
void ItemManager::removeItemAt(int y, int x) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active && items[i].y == y && items[i].x == x) {
            items[i].active = false;

            // map에 아직 아이템 값이 남아 있으면 빈칸 처리
            if (map[y][x] == items[i].type) {
                map[y][x] = EMPTY;
            }

            return;
        }
    }
}

// 특정 좌표가 아이템 칸인지 확인
bool ItemManager::isItemCell(int y, int x) const {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active && items[i].y == y && items[i].x == x) {
            return true;
        }
    }

    return false;
}