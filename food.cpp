// food.cpp
// Implements Growth Item metadata and effect values.
#include "food.h"

#include "cell_types.h"

int Food::type() {
    return GROWTH_ITEM;
}

int Food::spawnWeight() {
    return 45;
}

int Food::lengthDelta() {
    return 1;
}
