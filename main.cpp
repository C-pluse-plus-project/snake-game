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
    bool started;
    int currentStage;
    static const int LAST_STAGE = 4;

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

    void nextStage() {
        if (currentStage >= LAST_STAGE) {
            running = false;
            return;
        }

        currentStage++;
        board.loadMap(currentStage);
        snake.loadFromMap();
        items = ItemManager();
        score = ScoreManager();
        started = false;
        nodelay(stdscr, TRUE);
    }

    void handleInput() {
        const int key = getch();

        if (key == 'q' || key == 'Q') {
            running = false;
            return;
        }
        
        // 다음 맵으로 넘어가는 치트키(비활성화 해놓음)
        // if (key == 'c' || key == 'C') {
        //   nextStage();
        //   return;
        // }

        if (snake.missionClear) {
            if (currentStage < LAST_STAGE && (key == 'n' || key == 'N')) {
                nextStage();
            }
            return;
        }

        if (snake.gameOver) {
            return;
        }

        if (snake.turn(key)) {
            started = true;
        }
    }

    void update() {
        if (!started) {
            return;
        }

        if (snake.gameOver || snake.missionClear) {
            return;
        }

        items.update();
        snake.move(score, items);
    }

    void drawInfo() const {
        const int y = board.getSize();

        mvprintw(y + 3, 1, "Stage: %d / %d", currentStage, LAST_STAGE);
        mvprintw(y + 4, 1, "Arrows/WASD: move | q: quit");
        mvprintw(y + 5, 1, "# Wall  X Immune  G Gate  + Growth  - Poison  S Speed");
        mvprintw(y + 6, 1, "Direction: %c | Speed: %dms", snake.dir, snake.speed);
    }

    void render() const {
        clear();

        board.draw();

        const int scoreBoardX = board.getSize() * 2 + 5;
        score.draw(scoreBoardX, snake.length);
        drawInfo();

        if (snake.gameOver) {
            mvprintw(board.getSize() + 7, 1, "Game Over - press q to exit");
            mvprintw(board.getSize() + 8, 1, "Reason: %s", snake.gameOverReason);
        }
        else if (snake.missionClear) {
            if (currentStage < LAST_STAGE) {
                mvprintw(board.getSize() + 8, 1, "Mission Clear - press n for next stage");
            }
            else {
                mvprintw(board.getSize() + 8, 1, "All Stages Clear - press q to exit");
            }
        }
        else if (!started) {
            mvprintw(board.getSize() + 7, 1, "Press an arrow key or WASD to start");
        }

        refresh();
    }

public:
    GameManager() : running(true), started(false), currentStage(1) { snake.loadFromMap(); }

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
