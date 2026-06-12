// poison.h
// Declares the Poison class used for Poison Item behavior.
#ifndef POISON_H
#define POISON_H

class Poison {
public:
    static int type();
    static int spawnWeight();
    static int lengthDelta();
    static bool isFatalLength(int length);
};

#endif
