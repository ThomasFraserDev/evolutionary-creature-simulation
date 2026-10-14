# Evolutionary Creature Simulation

[![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![raylib](https://img.shields.io/badge/raylib-5.x-3F7FBF?style=for-the-badge)](https://www.raylib.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?style=for-the-badge)](./LICENSE)

## Description

This project is a simple evolutionary life simulation built in C++ with raylib. A population of creatures moves around the window, searches for food, avoids rocks, competes with one another, reproduces with genetic mutation, and eventually dies if their energy runs out.

The simulation is driven by a lightweight genetics system:

- Each creature has a genome controlling speed, acceleration, vision, size, metabolism, and aggression.
- Energy is consumed over time and replenished by eating food.
- Creatures switch between wandering, seeking food, and hunting based on energy and nearby creatures.
- Larger creatures can consume smaller ones when conditions allow.
- Reproduction creates offspring with mutated traits, allowing populations to evolve over time.
- The user can inspect a selected creature and toggle a population graph.

The core simulation logic lives in [`src/Simulation.cpp`](./src/Simulation.cpp) and the creature data model is defined in [`src/Creature.h`](./src/Creature.h) and [`src/Genome.h`](./src/Genome.h).

## Features

- Real-time simulation loop with raylib rendering
- Procedural food spawning and obstacle placement
- Creature behaviour states: wander, seek food, hunt
- Reproduction with mutation of genome values
- Population history graph
- Mouse-based creature selection for detailed stats
- Simple evolutionary pressure from resource scarcity and competition

## Project Structure

```text
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── build/
├── src/
│   ├── Behaviour.h
│   ├── Creature.h
│   ├── Food.h
│   ├── Genome.cpp
│   ├── Genome.h
│   ├── Rock.h
│   ├── Simulation.cpp
│   ├── Simulation.h
│   └── main.cpp
└──
```

## Prerequisites

- C++17 compatible compiler
- CMake 3.15 or newer
- raylib installed and available to CMake

On macOS, raylib can commonly be installed with Homebrew:

```bash
brew install raylib
```

## Download and Build

```bash
git clone https://github.com/ThomasFraserDev/simulation.git
cd simulation
cmake -S . -B build
cmake --build build
```

## Run

From the project root:

```bash
./build/simulation
```

The window opens at 1500x1000 and runs the simulation in real time.

## Controls

- Left-click on a creature to track it
- Press `G` to toggle the population history graph

## License

This software is licensed under the MIT license.
