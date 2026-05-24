/*
 * main.cpp
 * Snake Game 실행 파일입니다.
 */

#include <ncurses.h>
#include "board.h"

void drawScoreBoard(const int startX) {
    mvprintw(1, startX, "Score Board");
    mvprintw(3, startX, "B: 3 / 3");
    mvprintw(4, startX, "+: 0");
    mvprintw(5, startX, "-: 0");
    mvprintw(6, startX, "G: 0");

    mvprintw(9, startX, "Mission");
    mvprintw(11, startX, "B: 10 ( )");
    mvprintw(12, startX, "+: 5  ( )");
    mvprintw(13, startX, "-: 2  ( )");
    mvprintw(14, startX, "G: 1  ( )");
}

void drawInfo(const int boardSize) {
    mvprintw(boardSize + 3, 1, "0 Empty, 1 Wall, 2 Immune Wall, 3 Head, 4 Body");
    mvprintw(boardSize + 4, 1, "Press q to exit");
}

int main() {
    Board board;

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    resize_term(30, 70);

    while (true) {
        clear();

        board.draw();

        const int scoreBoardX = board.getSize() * 2 + 5;
        drawScoreBoard(scoreBoardX);
        drawInfo(board.getSize());

        refresh();

        const int key = getch();

        if (key == 'q') {
            break;
        }
    }

    endwin();

    return 0;
}
