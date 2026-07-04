#include <raylib.h>
#include <vector>

struct Creature {
    Vector2 position;
    float radius;
    Color color;
};

int main() {
InitWindow(800, 600, "simulation");
SetTargetFPS(60);

std::vector<Creature> creatures = {
    {{100, 100}, 12.0f, RED},
    {{200, 150}, 18.0f, BLUE},
    {{300, 220}, 10.0f, GREEN}
};

while (!WindowShouldClose()) {
    for (Creature& creature : creatures) {
        creature.position.x += 0.5f;
    }
BeginDrawing();
ClearBackground(RAYWHITE);
for (const Creature& creature : creatures) {
    DrawCircleV(creature.position, creature.radius, creature.color);
};
EndDrawing();
}

CloseWindow();
return 0;
}