#include <raylib.h>
#include <vector>

struct Creature {
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
};

struct Food {
    Vector2 position;
    float size;
    Color color;
};

int main() {
InitWindow(800, 600, "simulation");
SetTargetFPS(60);

std::vector<Creature> creatures = { // Create 3 creatures and their properties
    {{100, 100}, {0.5f, 0.0f}, 15.0f, RED},
    {{200, 150}, {0.0f, 0.5f}, 15.0f, BLUE},
    {{300, 220}, {-0.5f, 0.0f}, 15.0f, GREEN}
};

std::vector<Food> food = { // Create 3 pieces of food with their properties 
    {{150, 120}, 9.0f, GOLD},
    {{260, 200}, 9.0f, GOLD},
    {{400, 300}, 9.0f, GOLD}
};

while (!WindowShouldClose()) { // Main loop

    for (Creature& creature : creatures) {
        if (GetRandomValue(0, 100) < 2) { // If random chance of staying still isn't triggered 
            creature.velocity.x = (GetRandomValue(-100, 100) / 100.0f) * 2.0f; // Move random direction on the x axis
            creature.velocity.y = (GetRandomValue(-100, 100) / 100.0f) * 2.0f; // Move random direction on the y axis
        }

        creature.position.x += creature.velocity.x;
        creature.position.y += creature.velocity.y;

        // Bounce off of borders of window
        if (creature.position.x - creature.radius < 0) {
            creature.position.x = creature.radius;
            creature.velocity.x *= -1;
        }
        if (creature.position.x + creature.radius > 800) {
            creature.position.x = 800 - creature.radius;
            creature.velocity.x *= -1;
        }
        if (creature.position.y - creature.radius < 0) {
            creature.position.y = creature.radius;
            creature.velocity.y *= -1;
        }
        if (creature.position.y + creature.radius > 600) {
            creature.position.y = 600 - creature.radius;
            creature.velocity.y *= -1;
        }
    }

BeginDrawing();
ClearBackground(RAYWHITE);

for (const Creature& creature : creatures) {
    DrawCircleV(creature.position, creature.radius, creature.color); // Draw each creature with their properties 
}

for (const Food& f : food) {
    DrawRectangleV(f.position, {f.size, f.size} , f.color); // Draw each item of food with their properties 
}

EndDrawing();
}

CloseWindow();
return 0;
}