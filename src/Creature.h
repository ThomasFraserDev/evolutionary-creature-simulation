#pragma once

#include <raylib.h>

#include "Genome.h"
#include "Behaviour.h"

struct Creature {
    Vector2 position;
    Vector2 velocity;

    float radius;
    Genome genome;
    int energy; // Creature energy, depletes over time until it reaches 0

    float lastReproduction; // Time since creature last reproduced 
    float deathTime; // Time it takes creature to finalise death

    Behaviour behaviour;

    Vector2 targetPos;
    float behaviourUntil;
};