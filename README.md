# Dynamic Maze

A web-based dynamic maze game built in **C++20 + Raylib**, compiled to **WebAssembly**.

## Play
🎮 **[Play in Browser](https://kartikverma29.github.io/Dynamic_Maze/)** ← add your deployment URL

---

## Game Modes

| Mode | Description |
|------|-------------|
| **Dynamic Maze** | Maze shifts while you navigate — find the exit |
| **Dark Maze** | Limited visibility, maze may be static or dynamic |
| **Gauntlet** | Survival — enemies chase you through a maze |

---

## Controls
```
Arrow Keys / WASD — Move player
```

---

## Tech Stack
- **C++20** — game logic
- **Raylib 6.0** — rendering + input
- **raygui** — UI / menus
- **Emscripten** — compiled to WebAssembly

---

## Architecture
- Full **OOP + SOLID** design — 39 classes across 4 levels
- **Observer pattern** — event-driven gameplay (wall changes, collisions)
- **Strategy pattern** — swappable maze generators, pathfinders, score calculators
- **State pattern** — mode manager handles game state transitions
- **Adapter + Facade** — Raylib abstracted behind `IRenderer`
- **Dependency Injection** — no singletons

---

## Build (Native)
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
./dynamic_maze
```

## Build (Web)
```bash
mkdir build-web && cd build-web
emcmake cmake ..
make -j$(nproc)
```

---

## Project Structure
```
src/
├── core/       — Position, Direction, Wall, Cell
├── events/     — EventManager, typed events
├── maze/       — generation, mutation, solvability
├── player/     — Player, InputHandler, Lives
├── enemy/      — Chaser, Patrol, Blocker + pathfinding
├── scoring/    — per-mode score calculators
├── collision/  — event-based collision detection
├── modes/      — DynamicMaze, DarkMaze, Gauntlet, MainMenu
├── loop/       — GameLoop, ModeManager
├── renderer/   — IRenderer, RaylibRenderer
└── ui/         — UIManager (raygui)
```

---
