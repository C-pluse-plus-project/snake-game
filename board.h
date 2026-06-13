// board.h
// Declares the Board class and shared map size used by the game screen.
#ifndef BOARD_H
#define BOARD_H

#include "cell_types.h"

#include <ncurses.h>

class Board {
private:
    int cells[BOARD_SIZE][BOARD_SIZE];

public:
    Board();

    void loadMap(const int stage);

    int getSize() const;
    int getCell(const int y, const int x) const;
    void setCell(const int y, const int x, const int value);

    bool isInside(const int y, const int x) const;
    bool isWall(const int y, const int x) const;
    void draw() const;
};

#endif
