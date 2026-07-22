#include "Genome.h"
#include <raylib.h>

float Clamp(float value, float min, float max)
{
    if (value < min)
        return min;

    if (value > max)
        return max;

    return value;
}

void MutateGenome(Genome& genome) {
    genome.speed += GetRandomValue(-10,10) / 100.0f;

    genome.acceleration += GetRandomValue(-10,10) / 100.0f;

    genome.vision += GetRandomValue(-20,20);

    genome.metabolism += GetRandomValue(-10,10) / 100.0f;

    genome.aggression += GetRandomValue(-20,20);

    genome.speed = Clamp(
        genome.speed,
        0.1f,
        10.0f
    );

    genome.acceleration = Clamp(
        genome.acceleration,
        0.05f,
        2.0f
    );

    genome.vision = Clamp(
        genome.vision,
        50,
        1000
    );

    genome.metabolism = Clamp(
        genome.metabolism,
        0.1f,
        5.0f
    );

    genome.aggression = Clamp(
        genome.aggression,
        0,
        genome.maxEnergy
    );
}