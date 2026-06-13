// gate.h
// Declares the Gate class that creates gate pairs on normal walls and
// resolves snake movement through a gate according to the project rules.
#ifndef GATE_H
#define GATE_H

class Board;

class Gate {
public:
    static void placePair(Board& board);
    static bool moveThrough(const Board& board,
                            const int entryX,
                            const int entryY,
                            const char currentDir,
                            int& exitX,
                            int& exitY,
                            char& exitDir,
                            const char*& errorReason);
};

#endif
