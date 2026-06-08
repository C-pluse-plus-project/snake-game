#ifndef BOARD_H
#define BOARD_H

#include <ncurses.h>

const int BOARD_SIZE = 21;
extern int map[BOARD_SIZE][BOARD_SIZE];

class Board {
public:
    Board();

    void loadMap(int stage);

    int getSize() const;
    int getCell(const int y, const int x) const;
    void setCell(const int y, const int x, const int value);

    bool isWall(const int y, const int x) const;
    void draw() const;
};

#endif
