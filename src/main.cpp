#include <raylib.h>
#include <vector>
#include <cmath>

enum class Behaviour { // Class for creature behaviour 
    Wander,
    SeekFood,
    Hunt
};

struct Genome {
    float speed; // Creature movement speed
    float vision; // Vision range of the creature, for detecting food etc. 
    Color color;
};

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

struct Food {
    Vector2 position;
    float size;
    Color color;
};

int main() {
    InitWindow(1500, 1000, "Simulation");
    SetTargetFPS(60);

    std::vector<Creature> creatures; 
    const int creatureCount = 20;

    Color colors[] = { RED, BLUE, GREEN, PURPLE, PINK, ORANGE, YELLOW, SKYBLUE, VIOLET, MAROON, DARKGREEN, DARKBLUE };

    for (int i = 0; i < creatureCount; ++i) { // Create initial creatures
        Creature creature {
            {static_cast<float>(GetRandomValue(20, 1480)), static_cast<float>(GetRandomValue(20, 980))}, // Position
            {GetRandomValue(-100, 100) / 100.0f, GetRandomValue(-100, 100) / 100.0f }, // Velocity
            15.0f, // Radius

            {static_cast<float>(GetRandomValue(300, 500)), // Vision
            GetRandomValue(10, 25) / 10.0f, // Speed
            colors[GetRandomValue(0, 11)], // Colour
            },

            1400, // Energy
            0.0f, // Time since last reproduction
            0.0f, // Time the creature has been in a dying state for
            Behaviour::Wander, // Behaviour
            {0.0f, 0.0f}, // Target position
            0.0f // Behaviour timer
        };
        creatures.push_back(creature);
    }

    std::vector<Food> food; // Create initial food
    const int foodCount = 25;

    for (int i = 0; i < foodCount; ++i) {
        Food foodPiece {
            {static_cast<float>(GetRandomValue(20, 1480)),static_cast<float>(GetRandomValue(20, 980))}, // Position
            9.0f, // Size
            GOLD // Colour
        };
        food.push_back(foodPiece);
    }

    float lastFoodSpawnTime = 0.0; // Value for tracking time since last food spawn
    int trackedCreature = -1;

    while (!WindowShouldClose()) { // Main loop

        if (GetTime() - lastFoodSpawnTime >= 3.0) {
            for (int i = 0; i < 12; ++i) { // Spawn 12 new pieces of food every 3s
                food.push_back({{static_cast<float>(GetRandomValue(20, 1480)), static_cast<float>(GetRandomValue(20, 980))}, 9.0f, GOLD});
            }
            lastFoodSpawnTime = GetTime();
        }

        // Creature behaviour

        for (Creature& creature : creatures) {
            float now = GetTime();
            int closestFood = -1; // Index of closest food to creature, set to none by default
            int closestCreature = -1;
            float detectionRange = creature.genome.vision * creature.genome.vision; // Squared range of vision
            float closestDist = detectionRange; // Distance of the closest food

            if (creature.energy <= 300) {
                creature.behaviour = Behaviour::Hunt;
            } else if (creature.energy <= 700) {
                creature.behaviour = Behaviour::SeekFood;
            } else if (now >= creature.behaviourUntil) {
                creature.behaviour = Behaviour::Wander;
                creature.targetPos = {
                    static_cast<float>(GetRandomValue(20, 1480)),
                    static_cast<float>(GetRandomValue(20, 980))
                };
                creature.behaviourUntil = now + GetRandomValue(2, 5);
            }
            if (creature.energy <= 300) {
                for (int i = 0; i < creatures.size(); ++i) {
                    if ((creature.position.x != creatures[i].position.x) && (creature.position.y != creatures[i].position.y) && (creature.radius > creatures[i].radius)) {
                        float dx = creatures[i].position.x - creature.position.x;
                        float dy = creatures[i].position.y - creature.position.y;
                        float dist = dx * dx + dy * dy; // Calculate distance between creature and each other smaller creature
                
                        if (dist < detectionRange && dist < closestDist) { // Check if smaller creature is the closest one to the creature
                            closestDist = dist;
                            closestCreature = i;
                        }
                    }
                    if (closestCreature != -1) { // If there's a closest creature
                        float dx = creatures[closestCreature].position.x - creature.position.x;
                        float dy = creatures[closestCreature].position.y - creature.position.y;
                        float len = sqrt(dx * dx + dy * dy); // Calculate and normalize direction for movement
                        if (len > 0.0001f) {
                            creature.behaviour = Behaviour::Hunt;
                            float targetVx = (dx / len) * creature.genome.speed;
                            float targetVy = (dy / len) * creature.genome.speed;
                            creature.velocity.x = creature.velocity.x * 0.9f + targetVx * 0.1f; // Turn creature towards direction of target creature
                            creature.velocity.y = creature.velocity.y * 0.9f + targetVy * 0.1f;
                        }
                    }
                }
            }

            Vector2 desired = creature.velocity;

            for (int i = 0; i < food.size(); ++i) {
                float dx = food[i].position.x - creature.position.x;
                float dy = food[i].position.y - creature.position.y;
                float dist = dx * dx + dy * dy; // Calculate distance between creature and each piece of food 
                
                if (dist < detectionRange && dist < closestDist) { // Check if food is the closest one to the creature
                    closestDist = dist;
                    closestFood = i;
                }
            }

            if (creature.behaviour == Behaviour::SeekFood && closestFood != -1) {
                float dx = food[closestFood].position.x - creature.position.x;
                float dy = food[closestFood].position.y - creature.position.y;
                float len = sqrt(dx * dx + dy * dy); // Calculate and normalize direction for movement
                if (len > 0.0001f) {
                    desired = {(dx / len) * creature.genome.speed, (dy / len) * creature.genome.speed};
                }
            }

            else if (creature.behaviour == Behaviour::Hunt && closestCreature != -1) {
                float dx = creatures[closestCreature].position.x - creature.position.x;
                float dy = creatures[closestCreature].position.y - creature.position.y;
                float len = sqrt(dx * dx + dy * dy); // Calculate and normalize direction for movement
                if (len > 0.0001f) {
                    desired = {(dx / len) * creature.genome.speed, (dy / len) * creature.genome.speed};
                }
            }

            else if (creature.behaviour == Behaviour::Wander) {
                float dx = creature.targetPos.x - creature.position.x;
                float dy = creature.targetPos.y - creature.position.y;
                float len = sqrt(dx * dx + dy * dy); // Calculate and normalize direction for movement
                if (len > 0.0001f) {
                    desired = { (dx / len) * creature.genome.speed, (dy / len) * creature.genome.speed };
                }
            }

            creature.velocity.x = creature.velocity.x * 0.9f + desired.x * 0.1f;
            creature.velocity.y = creature.velocity.y * 0.9f + desired.y * 0.1f;

            creature.position.x += creature.velocity.x;
            creature.position.y += creature.velocity.y;
            creature.energy -= 1 * creature.genome.speed;

            // Bounce off of borders of window
            if (creature.position.x - creature.radius < 0) {
                creature.position.x = creature.radius;
                creature.velocity.x *= -1;
            }
            if (creature.position.x + creature.radius > 1500) {
                creature.position.x = 1500 - creature.radius;
                creature.velocity.x *= -1;
            }
            if (creature.position.y - creature.radius < 0) {
                creature.position.y = creature.radius;
                creature.velocity.y *= -1;
            }
            if (creature.position.y + creature.radius > 1000) {
                creature.position.y = 1000 - creature.radius;
                creature.velocity.y *= -1;
            }
        }

        // Collisions

        int remove = -1;

        auto creatureCollision = [&](Creature& a, Creature& b, int aIndex, int bIndex) {
            float dx = b.position.x - a.position.x; // Calculate x distance between creatures
            float dy = b.position.y - a.position.y; // Calculate y distance between creatures

            float minDist = a.radius + b.radius; // Check for collision 
            float distSq = dx * dx + dy * dy;

            if (distSq >= minDist * minDist) {
                return; // Return if no collision
            }

            if (a.energy <= 300 && a.radius > b.radius) {
                a.energy += b.energy;
                a.radius += (b.radius / 2);
                remove = bIndex;
            }

            if (b.energy <= 300 && b.radius > a.radius) {
                b.energy += a.energy;
                b.radius += (a.radius / 2);
                remove = aIndex;
            }

            float dist = sqrtf(distSq);
            if (dist < 0.0001f) {
                dist = 0.0001f; // Avoid division by zero 
            }

            float overlap = minDist - dist; // Check how much overlap
            float nx = dx / dist;
            float ny = dy / dist;

            // Push creatures off of eachother
            a.position.x -= nx * overlap * (b.radius / (a.radius + b.radius));
            a.position.y -= ny * overlap * (b.radius / (a.radius + b.radius));
            b.position.x += nx * overlap * (a.radius / (a.radius + b.radius));
            b.position.y += ny * overlap * (a.radius / (a.radius + b.radius));

            std::swap(a.velocity, b.velocity); // Bounce creatures off of eachother 
        };

        for (int i = 0; i < creatures.size(); ++i) {
            for (int j = i + 1; j < static_cast<int>(creatures.size()); ++j) {
                creatureCollision(creatures[i], creatures[j], i, j); // Check for collision between each pair of creatures
            }
        }

        if (remove != -1 && remove < creatures.size()) {
            creatures.erase(creatures.begin() + remove);
        }

        for (Creature& creature : creatures) {
            for (int i = 0; i < food.size(); ++i) {
                if (CheckCollisionCircleRec(creature.position, creature.radius, {food[i].position.x, food[i].position.y, food[i].size, food[i].size})) { // Check if creature collides with food
                    creature.energy += (15000 / creature.radius); // Increase creature energy and size after collision, and remove the food afterwards. Energy gained is dependent on creature size
                    creature.radius += 3;
                    food.erase(food.begin() + i);
                    --i;
                }
            }
        }

        // Reproduction & death

        for (int i = 0; i < creatures.size(); ++i) {
            if (creatures[i].radius > 30.0f && GetTime() - creatures[i].lastReproduction > 15.0f) { // If big enough, a creature can create offspring every 6s
                Creature offspring = { // Create offspring at an offset from its parent's position, with modifications of their properties
                    {creatures[i].position.x + GetRandomValue(-10, 10), creatures[i].position.y + GetRandomValue(-10, 10)},
                    {0.0f, 0.0f},
                    static_cast<float>(creatures[i].radius * 0.4),
                    creatures[i].genome,
                    700,
                    static_cast<float>(GetTime()),
                    0.0f,
                    Behaviour::Wander,
                    {0.0f, 0.0f},
                    0.0f
                };
                offspring.genome.speed += GetRandomValue(-30, 30) / 100.0f;
                offspring.genome.vision += GetRandomValue(-10, 10);
                creatures.push_back(offspring);
                creatures[i].lastReproduction = GetTime(); // Reset reproduction timer
                creatures[i].energy -= 400;
                creatures[i].radius *= 0.85f;
            }

            if (creatures[i].energy <= 0) {  // When a creature's energy depletes to 0
                if (creatures[i].deathTime == 0.0f) {
                    creatures[i].deathTime = GetTime(); // Get time stamp of when creature begins to die 
                }

                creatures[i].radius *= 0.98f; // Deplete creature size
                if (GetTime() - creatures[i].deathTime > 0.5f) { // After 0.5s of dying, remove the creature
                    creatures.erase(creatures.begin() + i);
                    --i;  // Adjust index after removal
                }
            }
        }

        Vector2 mouse = GetMousePosition();
        
        for (int i = 0; i < creatures.size(); ++i) {
            float dx = mouse.x - creatures[i].position.x;
            float dy = mouse.y - creatures[i].position.y;
            float distSq = dx * dx + dy * dy;

            if (distSq < creatures[i].radius * creatures[i].radius && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                trackedCreature = i;
            }
        }
        if (trackedCreature != -1) {
            const char* behaviourText = "Wandering...";
            if (creatures[trackedCreature].behaviour == Behaviour::SeekFood) {
                behaviourText = "Seeking food...";
            } else if (creatures[trackedCreature].behaviour == Behaviour::Hunt) {
                behaviourText = "Hunting...";
            }

            DrawText(
                TextFormat(
                    "Behaviour: %s\nEnergy: %d\nSpeed: %.2f\nRadius: %.2f\nVision: %.2f",
                    behaviourText,
                    creatures[trackedCreature].energy,
                    creatures[trackedCreature].genome.speed,
                    creatures[trackedCreature].radius,
                    creatures[trackedCreature].genome.vision
                ),
                20,20,20,creatures[trackedCreature].genome.color
            );
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (const Creature& creature : creatures) {
            DrawCircleV(creature.position, creature.radius, creature.genome.color); // Draw each creature with their properties 
        }

        for (const Food& f : food) {
            DrawRectangleV(f.position, {f.size, f.size} , f.color); // Draw each item of food with their properties 
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
