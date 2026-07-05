#include <raylib.h>
#include <vector>

struct Creature {
    Vector2 position;
    Vector2 velocity;
    float radius;
    int energy;
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
    {{100, 100}, {0.5f, 0.0f}, 15.0f, 1000, RED},
    {{200, 150}, {0.0f, 0.5f}, 15.0f, 1000, BLUE},
    {{300, 220}, {-0.5f, 0.0f}, 15.0f, 1000, GREEN}
};

std::vector<Food> food = { // Create 3 pieces of food with their properties 
    {{static_cast<float>(GetRandomValue(50, 750)), static_cast<float>(GetRandomValue(50, 550))}, 9.0f, GOLD},
    {{static_cast<float>(GetRandomValue(50, 750)), static_cast<float>(GetRandomValue(50, 550))}, 9.0f, GOLD},
    {{static_cast<float>(GetRandomValue(50, 750)), static_cast<float>(GetRandomValue(50, 550))}, 9.0f, GOLD}
};

float lastFoodSpawnTime = 0.0;

while (!WindowShouldClose()) { // Main loop

    if (GetTime() - lastFoodSpawnTime >= 10.0) {
        for (int i = 0; i < 3; ++i) { // Spawn 3 new pieces of food every 10s
            food.push_back({{static_cast<float>(GetRandomValue(50, 750)), static_cast<float>(GetRandomValue(50, 550))}, 9.0f, GOLD});
        }
        lastFoodSpawnTime = GetTime();
    }

    for (Creature& creature : creatures) {
        if (GetRandomValue(0, 100) < 2) { // If random chance of staying still isn't triggered 
            creature.velocity.x = (GetRandomValue(-100, 100) / 100.0f) * 2.0f; // Move random direction on the x axis
            creature.velocity.y = (GetRandomValue(-100, 100) / 100.0f) * 2.0f; // Move random direction on the y axis
        }

        creature.position.x += creature.velocity.x;
        creature.position.y += creature.velocity.y;
        creature.energy -= 1;

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

    for (Creature& creature : creatures) {
        for (int i = 0; i < food.size(); ++i) {
             if (CheckCollisionCircleRec(creature.position, creature.radius, {food[i].position.x, food[i].position.y, food[i].size, food[i].size})) { // Check if creature collides with food
                creature.energy += 1000; // Increase creature energy and size after collision, and remove the food
                creature.radius += 5;
                food.erase(food.begin() + i);
                --i;
            }
        }
    }

    for (int i = 0; i < creatures.size(); ++i) {
        if (creatures[i].energy <= 0) { 
            creatures.erase(creatures.begin() + i); // Remove a ceature when its energy depletes to 0
            --i;  // Adjust index after removal
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