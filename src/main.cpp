#include <raylib.h>
#include <vector>
#include <cmath>

struct Creature {
    Vector2 position;
    Vector2 velocity;
    float radius;
    float vision;
    float speed;
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

std::vector<Creature> creatures = { // Create 5 creatures and their properties
    {{100, 100}, {0.5f, 0.0f}, 15.0f, 130.0f, 2.0f, 1000, RED},
    {{200, 150}, {0.0f, 0.5f}, 15.0f, 150.0f, 1.5f, 1000, BLUE},
    {{300, 220}, {-0.5f, 0.0f}, 15.0f, 200.0f, 1.0f, 1000, GREEN},
    {{400, 360}, {-0.5f, 0.0f}, 15.0f, 170.0f, 1.67f, 1000, PURPLE},
    {{500, 420}, {-0.5f, 0.0f}, 15.0f, 160.0f, 1.3f, 1000, PINK}
};

std::vector<Food> food = { // Create 5 pieces of food with their properties 
    {{static_cast<float>(GetRandomValue(50, 750)), static_cast<float>(GetRandomValue(50, 550))}, 9.0f, GOLD},
    {{static_cast<float>(GetRandomValue(50, 750)), static_cast<float>(GetRandomValue(50, 550))}, 9.0f, GOLD},
    {{static_cast<float>(GetRandomValue(50, 750)), static_cast<float>(GetRandomValue(50, 550))}, 9.0f, GOLD},
    {{static_cast<float>(GetRandomValue(50, 750)), static_cast<float>(GetRandomValue(50, 550))}, 9.0f, GOLD},
    {{static_cast<float>(GetRandomValue(50, 750)), static_cast<float>(GetRandomValue(50, 550))}, 9.0f, GOLD}
};

float lastFoodSpawnTime = 0.0;

while (!WindowShouldClose()) { // Main loop

    if (GetTime() - lastFoodSpawnTime >= 10.0) {
        for (int i = 0; i < 5; ++i) { // Spawn 5 new pieces of food every 10s
            food.push_back({{static_cast<float>(GetRandomValue(50, 750)), static_cast<float>(GetRandomValue(50, 550))}, 9.0f, GOLD});
        }
        lastFoodSpawnTime = GetTime();
    }

    for (Creature& creature : creatures) {
        int closestFood = -1; // Index of closest food to creature, set to none by default
        float detectionRange = creature.vision * creature.vision; // Squared range of vision
        float closestDist = detectionRange; // Distance of the closest food

        for (int i = 0; i < food.size(); ++i) {
            float dx = food[i].position.x - creature.position.x;
            float dy = food[i].position.y - creature.position.y;
            float dist = dx * dx + dy * dy; // Calculate distance between creature and each piece of food 
            
            if (dist < detectionRange && dist < closestDist) { // Check if food is the closest one to the creature
                closestDist = dist;
                closestFood = i;
            }
        }
        if (closestFood != -1) { // If there's a closest piece of food 
            float dx = food[closestFood].position.x - creature.position.x;
            float dy = food[closestFood].position.y - creature.position.y;
            float len = sqrt(dx * dx + dy * dy); // Calculate and normalize direction for movement
            creature.velocity.x = (dx / len) * creature.speed;
            creature.velocity.y = (dy / len) * creature.speed;
        }

        else if (GetRandomValue(0, 100) < 2) { // If no enearby food and random chance of staying still isn't triggered 
            creature.velocity.x = (GetRandomValue(-100, 100) / 100.0f) * creature.speed; // Move random direction on the x axis
            creature.velocity.y = (GetRandomValue(-100, 100) / 100.0f) * creature.speed; // Move random direction on the y axis
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