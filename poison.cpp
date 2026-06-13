// poison.cpp
// Implements Poison Item metadata and failure rule.
#include "poison.h"

#include "cell_types.h"

int Poison::type() {
    return POISON_ITEM;
}

int Poison::spawnWeight() {
    return 45;
}

int Poison::lengthDelta() {
    return -1;
}

bool Poison::isFatalLength(const int length) {
    return length < 3;
}
