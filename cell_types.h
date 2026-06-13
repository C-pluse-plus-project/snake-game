// cell_types.h
// Defines board size and cell values shared by the game classes.
#ifndef CELL_TYPES_H
#define CELL_TYPES_H

constexpr int BOARD_SIZE = 21;

constexpr int EMPTY = 0;
constexpr int WALL = 1;
constexpr int IMMUNE_WALL = 2;
constexpr int SNAKE_HEAD = 3;
constexpr int SNAKE_BODY = 4;
constexpr int GATE = 5;
constexpr int GROWTH_ITEM = 6;
constexpr int POISON_ITEM = 7;
constexpr int SPEED_ITEM = 8;
constexpr int TEMP_WALL = 9;
constexpr int TEMP_WALL_READY = 10;

#endif
