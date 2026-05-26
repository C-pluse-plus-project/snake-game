/*
 * board.h
 * Snake Game의 게임판(Board) 클래스를 선언하는 파일입니다.
 * 맵 데이터 저장, 셀 조회/수정, 벽 판정, 맵 출력 기능을 제공합니다.
 */

#ifndef BOARD_H
#define BOARD_H

#include <ncurses.h>

class Board {
private:
    static const int SIZE = 21;
    int map[SIZE][SIZE];

public:
    Board();

    int getSize() const;
    int getCell(const int y, const int x) const;
    void setCell(const int y, const int x, const int value);

    bool isWall(const int y, const int x) const;
    void draw() const;
};

#endif
