#pragma once

#include <raylib.h>

struct Genome {
    float speed;  // Creature movement speed
    float vision; // Vision range of the creature, for detecting food etc. 
    Color color;
};