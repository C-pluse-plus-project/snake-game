#include "board.h"

int map[BOARD_SIZE][BOARD_SIZE];

Board::Board() {
    const int initialMap[BOARD_SIZE][BOARD_SIZE] = {
        {2,1,1,1,1,1,1,1,1,1,5,1,1,1,1,1,1,1,1,1,2},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,3,4,4,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2}
    };

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            map[y][x] = initialMap[y][x];
        }
    }
}

int Board::getSize() const {
    return BOARD_SIZE;
}

int Board::getCell(const int y, const int x) const {
    return map[y][x];
}

void Board::setCell(const int y, const int x, const int value) {
    map[y][x] = value;
}

bool Board::isWall(const int y, const int x) const {
    return map[y][x] == 1 || map[y][x] == 2;
}

void Board::draw() const {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            char ch = ' ';

            if (map[y][x] == 1) ch = '#';
            else if (map[y][x] == 2) ch = 'X';
            else if (map[y][x] == 3) ch = 'H';
            else if (map[y][x] == 4) ch = 'B';
            else if (map[y][x] == 5) ch = 'G';
            else if (map[y][x] == 6) ch = '+';
            else if (map[y][x] == 7) ch = '-';
            else if (map[y][x] == 8) ch = 'S';
            else if (map[y][x] == 9) ch = '~';

            mvprintw(y + 1, x * 2 + 1, "%c ", ch);
        }
    }
}
