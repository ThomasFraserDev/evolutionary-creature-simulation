#pragma once

#include <vector>

#include "Creature.h"
#include "Food.h"

class Simulation {
public:

    Simulation();

    void Update();
    void Draw();

private:

    void SpawnFood();

    void UpdateCreatures();

    void HandleCreatureCollisions();

    void HandleFoodCollisions();

    void HandleReproduction();

    void HandlePop();

    void HandleDeaths();

    void HandleMouseSelection();

private:

    std::vector<Creature> creatures;
    std::vector<Food> food;

    float lastFoodSpawnTime = 0.0f;

    int trackedCreature = -1;
};