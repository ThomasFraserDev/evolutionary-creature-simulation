#include <raylib.h>

#include "Simulation.h"

int main() {
    InitWindow(1500, 1000, "Simulation");

    SetTargetFPS(60);

    Simulation simulation;

    while (!WindowShouldClose()) {
        simulation.Update();

        BeginDrawing();

        ClearBackground(BLACK);

        simulation.Draw();

        EndDrawing();
    }

    CloseWindow();
}