#pragma once

#include <raylib.h>

struct Genome {
    float speed;  // Creature movement speed
    float acceleration;
    float vision; // Vision range of the creature, for detecting food etc. 
    float size;
    float metabolism;
    float maxEnergy;
    float aggression; // Threshold for when the creature becomes aggresive
    Color color;
};

void MutateGenome(Genome& genome);