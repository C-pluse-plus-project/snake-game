#include <ncurses.h>
#include "board.h"
#include "item_score.h"
#include "snake_logic.h"

class GameManager {
private:
    Board board;
    snake_logic snake;
    ItemManager items;
    ScoreManager score;
    bool running;

    void initScreen() {
        initscr();
        cbreak();
        noecho();
        curs_set(0);
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        resize_term(30, 70);
    }

    void closeScreen() {
        endwin();
    }

    void handleInput() {
        const int key = getch();

        if (key == 'q' || key == 'Q') {
            running = false;
            return;
        }

        snake.turn(key);
    }

    void update() {
        if (snake.gameOver || snake.missionClear) {
            nodelay(stdscr, FALSE);
            return;
        }

        items.update();
        snake.move(score, items);
    }

    void drawInfo() const {
        const int y = board.getSize();

        mvprintw(y + 3, 1, "Arrows/WASD: move | q: quit");
        mvprintw(y + 4, 1, "# Wall  X Immune  G Gate  + Growth  - Poison  S Speed");
        mvprintw(y + 5, 1, "Direction: %c | Speed: %dms", snake.dir, snake.speed);
    }

    void render() const {
        clear();

        board.draw();

        const int scoreBoardX = board.getSize() * 2 + 5;
        score.draw(scoreBoardX, snake.length);
        drawInfo();

        if (snake.gameOver) {
            mvprintw(board.getSize() + 7, 1, "Game Over - press q to exit");
        }
        else if (snake.missionClear) {
            mvprintw(board.getSize() + 7, 1, "Mission Clear - press q to exit");
        }

        refresh();
    }

public:
    GameManager() : running(true) {}

    void run() {
        initScreen();

        while (running) {
            handleInput();
            update();
            render();
            napms(10);
        }

        closeScreen();
    }
};

int main() {
    GameManager game;
    game.run();

    return 0;
}
