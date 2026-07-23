#pragma once

#include <vector>

#include "Creature.h"
#include "Food.h"
#include "Rock.h"

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

    void HandleRockCollisions();

    void HandleReproduction();

    void HandlePop();

    void HandleDeaths();

    void HandleMouseSelection();

private:

    std::vector<Creature> creatures;
    std::vector<Food> food;
    std::vector<Rock> rocks;

    float lastFoodSpawnTime = 0.0f;

    int trackedCreature = -1;

    std::vector<int> populationHistory;
    double lastGraphUpdate = 0.0;
};