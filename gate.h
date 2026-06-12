// gate.h
// Declares the Gate class that creates gate pairs on normal walls and
// resolves snake movement through a gate according to the project rules.
#ifndef GATE_H
#define GATE_H

class Gate {
public:
    static void placePair();
    static bool moveThrough(int entryX,
                            int entryY,
                            char currentDir,
                            int& exitX,
                            int& exitY,
                            char& exitDir,
                            const char*& errorReason);
};

#endif
