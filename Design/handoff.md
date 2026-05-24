# Dynamic Maze — Project Handoff
> Complete reference for resuming development at any point.

---

## Project Overview

A web-based dynamic maze game built in **C++ with Raylib**, compiled to **WebAssembly via Emscripten**.
The maze changes layout during gameplay based on defined rules while always remaining solvable.

---

## Tech Stack

| Component | Choice |
|-----------|--------|
| Language | C++17 |
| Game Engine | Raylib |
| Web Build | Emscripten (emcc) |
| Compiler | Clang / clang++ |
| Build System | CMake (native + web targets) |
| Hosting | GitHub Pages / Netlify |

---

## Game Modes

### Mode 1 — Dynamic Maze
- Shifting maze, find the exit
- 4–5 levels, maze size grows per level
- Score: time taken + path optimality vs optimal
- Enemies: optional (zero by default)
- Speed: fixed

### Mode 2 — Dark Maze
- Limited light radius around player, no map
- Player chooses dynamic or static maze variant
- 4–5 levels
- Score: time + path optimality + % map uncovered bonus
- Enemies: none
- Speed: fixed

### Mode 3 — Gauntlet
- Pac-Man meets dynamic maze — endless survival run
- Enemies scale with difficulty
- Score: time survived + enemies defeated
- Speed: increases over time
- Idle punishment: walls open around player OR patrol spawns nearby

---

## Core Game Rules (all modes)

- Maze changes after X steps (primary trigger) + random variance ±10–20 steps
- Changes target the path player is currently following — partial wall shifts, not full resets
- Solvability always maintained — at least one valid path exists at every state
- Player is never caged during a layout change
- 3 lives → game over (classical system)
- Player movement: free/smooth (not grid-snapped)
- View: top-down 2D

---

## Enemies (Gauntlet Only)

| Enemy | Defeat Condition | Notes |
|-------|-----------------|-------|
| Chaser | Cannot be defeated | BFS pathfinding, avoid only |
| Patrol | Collision from behind | Defeatable from opposite to movement direction |
| Blocker | Only when moving | Immortal when stationary |

- Attack mechanic: simple collision (no button press)
- Enemy count scales with difficulty

---

## Difficulty Scaling

| Mode | Scales On | Effect |
|------|-----------|--------|
| Dynamic / Dark | Stages solved | Maze size grows, change frequency increases |
| Gauntlet | Time survived | Player speed increases, more enemies spawn |

---

## Scoring

| Mode | Primary Score | Bonus |
|------|--------------|-------|
| Dynamic Maze | Time + path optimality | — |
| Dark Maze | Time + path optimality | % map uncovered |
| Gauntlet | Time survived | Enemies defeated |

- Online leaderboard (Firebase or Supabase — decided during build)

---

## Maze Structure

**Phase 1:**
- Grid maze (standard cell-based)
- Concentric ring maze (rings rotate + walls shift)

**Phase 2 (deferred):**
- Mixed grid + ring
- Asymmetric layouts

---

## Deferred Decisions (decide during build)

- Death screen design (score, retry, leaderboard view)
- Between-level score summary screen
- Leaderboard backend (Firebase vs Supabase)
- Which modes use grid vs ring maze structure
- Gauntlet starting speed baseline
- Enemy count per difficulty level (exact numbers)
- Power-ups / collectibles (ignored for MVP)

---

## Design Patterns Used

| Pattern | Applied To |
|---------|-----------|
| Observer | EventManager, IEventListener\<T\>, IEventPublisher\<T\> |
| Strategy | IMazeGenerator, ISolvabilityChecker, IPathfinder, IScoreCalculator |
| State | IAppState, ModeManager |
| Adapter + Facade | RaylibRenderer |
| Dependency Injection | All interfaces injected via constructor |

---

## Architecture Principles

- **SOLID** throughout — SRP, OCP, LSP, ISP, DIP
- **OOP** — no plain structs, everything as classes
- **Multi-file** — every class has its own `.h` and `.cpp`
- **`#pragma once`** for all header guards
- **Forward declarations** where possible to avoid circular includes
- **No raw owning pointers** — `unique_ptr` for ownership, raw `*` for borrowing only
- **Template interfaces** (`IEventListener<T>`) — implementation stays in header

---

## Complete Class List

### Level 1 — Atomic
`Position` `Direction` `Wall` `Cell` `PairHash`
`IEventListener<T>` `IEventPublisher<T>` `EventManager`

### Level 2 — Maze
`Maze` `IMazeGenerator` `RecursiveBacktrackerGenerator` `PrimsGenerator`
`ISolvabilityChecker` `BFSSolvabilityChecker` `DFSSolvabilityChecker`
`IMazeMutator` `GridMazeMutator`

### Level 3 — Player & Enemy
`Player` `InputHandler` `Lives`
`IScoreCalculator` `DynamicMazeScoreCalculator` `DarkMazeScoreCalculator` `GauntletScoreCalculator`
`IEnemy` `Chaser` `Patrol` `Blocker`
`IPathfinder` `BFSPathfinder` `PatrolPathfinder`
`CollisionDetector`

### Level 4 — Game Loop
`IAppState` `IGameMode` `DynamicMazeMode` `DarkMazeMode` `GauntletMode`
`MainMenuMode` `SettingsMode` `ModeManager` `GameLoop`
`IRenderer` `RaylibRenderer` `UIManager`

### Events (6 total)
`WallAddedEvent` `WallRemovedEvent` `WallStateChangedEvent`
`PlayerMovedEvent` `PlayerHitEvent` `EnemyDefeatedEvent`

---

## Event Flow

```
InputHandler
  --publishes--> PlayerMovedEvent
      └── listened by: Player, CollisionDetector

GridMazeMutator
  --publishes--> WallAddedEvent        → listened by: Cell
  --publishes--> WallRemovedEvent      → listened by: Cell
  --publishes--> WallStateChangedEvent → listened by: Cell, Player, IEnemy

CollisionDetector
  --publishes--> PlayerHitEvent        → listened by: Player, Lives
  --publishes--> EnemyDefeatedEvent    → listened by: GauntletScoreCalculator, Lives
```

---

## File / Module Structure

```
dynamic_maze/
├── CMakeLists.txt
├── assets/
│   ├── fonts/
│   ├── sounds/
│   └── textures/
├── web/
│   ├── index.html
│   ├── game.js
│   └── game.wasm
└── src/
    ├── main.cpp
    ├── core/
    │   ├── Position.h / .cpp
    │   ├── Direction.h / .cpp
    │   ├── Wall.h / .cpp
    │   └── Cell.h / .cpp
    ├── events/
    │   ├── IEventListener.h           (header only)
    │   ├── IEventPublisher.h          (header only)
    │   ├── EventManager.h / .cpp
    │   └── events/
    │       ├── WallAddedEvent.h       (header only)
    │       ├── WallRemovedEvent.h     (header only)
    │       ├── WallStateChangedEvent.h(header only)
    │       ├── PlayerMovedEvent.h     (header only)
    │       ├── PlayerHitEvent.h       (header only)
    │       └── EnemyDefeatedEvent.h   (header only)
    ├── maze/
    │   ├── PairHash.h                 (header only)
    │   ├── Maze.h / .cpp
    │   ├── generation/
    │   │   ├── IMazeGenerator.h       (header only)
    │   │   ├── RecursiveBacktrackerGenerator.h / .cpp
    │   │   └── PrimsGenerator.h / .cpp
    │   ├── solvability/
    │   │   ├── ISolvabilityChecker.h  (header only)
    │   │   ├── BFSSolvabilityChecker.h / .cpp
    │   │   └── DFSSolvabilityChecker.h / .cpp
    │   └── mutation/
    │       ├── IMazeMutator.h         (header only)
    │       └── GridMazeMutator.h / .cpp
    ├── player/
    │   ├── Player.h / .cpp
    │   ├── Lives.h / .cpp
    │   └── InputHandler.h / .cpp
    ├── enemy/
    │   ├── IEnemy.h / .cpp            (abstract)
    │   ├── Chaser.h / .cpp
    │   ├── Patrol.h / .cpp
    │   ├── Blocker.h / .cpp
    │   └── pathfinding/
    │       ├── IPathfinder.h          (header only)
    │       ├── BFSPathfinder.h / .cpp
    │       └── PatrolPathfinder.h / .cpp
    ├── scoring/
    │   ├── IScoreCalculator.h         (header only)
    │   ├── DynamicMazeScoreCalculator.h / .cpp
    │   ├── DarkMazeScoreCalculator.h / .cpp
    │   └── GauntletScoreCalculator.h / .cpp
    ├── collision/
    │   └── CollisionDetector.h / .cpp
    ├── modes/
    │   ├── IAppState.h                (header only)
    │   ├── IGameMode.h / .cpp         (abstract)
    │   ├── DynamicMazeMode.h / .cpp
    │   ├── DarkMazeMode.h / .cpp
    │   ├── GauntletMode.h / .cpp
    │   ├── MainMenuMode.h / .cpp
    │   └── SettingsMode.h / .cpp
    ├── core_loop/
    │   ├── ModeManager.h / .cpp
    │   └── GameLoop.h / .cpp
    ├── renderer/
    │   ├── IRenderer.h                (header only)
    │   └── RaylibRenderer.h / .cpp
    └── ui/
        └── UIManager.h / .cpp
```

---

## Implementation Order (Bottom-Up, 46 steps)

### Phase 1 — L1 Core & Events
1. `Position`
2. `Direction`
3. `Wall`
4. `IEventListener<T>` — header only
5. `IEventPublisher<T>` — header only
6. All 6 event structs — header only
7. `EventManager`
8. `Cell`

### Phase 2 — L2 Maze
9. `PairHash` — header only
10. `Maze`
11. `IMazeGenerator` — header only
12. `RecursiveBacktrackerGenerator`
13. `PrimsGenerator`
14. `ISolvabilityChecker` — header only
15. `BFSSolvabilityChecker`
16. `DFSSolvabilityChecker`
17. `IMazeMutator` — header only
18. `GridMazeMutator`

### Phase 3 — L3 Player & Enemy
19. `InputHandler`
20. `Player`
21. `Lives`
22. `IScoreCalculator` — header only
23. `DynamicMazeScoreCalculator`
24. `DarkMazeScoreCalculator`
25. `GauntletScoreCalculator`
26. `IPathfinder` — header only
27. `BFSPathfinder`
28. `PatrolPathfinder`
29. `IEnemy` — abstract
30. `Chaser`
31. `Patrol`
32. `Blocker`
33. `CollisionDetector`

### Phase 4 — L4 Game Loop & Modes
34. `IAppState` — header only
35. `IGameMode` — abstract
36. `IRenderer` — header only
37. `RaylibRenderer`
38. `UIManager`
39. `DynamicMazeMode`
40. `DarkMazeMode`
41. `GauntletMode`
42. `MainMenuMode`
43. `SettingsMode`
44. `ModeManager`
45. `GameLoop`
46. `main.cpp`

---

## Ownership Rules (Memory)

| Situation | Use |
|-----------|-----|
| Class owns another class | `unique_ptr<T>` |
| Class borrows (DI injected) | `T&` reference |
| Non-owning pointer to array/list | `T*` raw pointer |
| Maze owns Walls | `vector<unique_ptr<Wall>>` |
| Cell borrows Wall | `Wall*` raw pointer |
| ModeManager manages states | `map<AppStateType, IAppState*>` |

---

## What's Next

1. `CMakeLists.txt` — native + Emscripten build setup
2. Start coding Phase 1 (`Position` → `Cell`)
3. Test each class in isolation before moving up
4. Integrate Raylib at Phase 4 (`RaylibRenderer` only)

---

## Key Constraints to Remember

- Game code **never calls Raylib directly** — only through `IRenderer`
- `IPathfinder::findPath()` is **stateless** — `Maze&` passed each call
- `GridMazeMutator` validates **per-wall** within a batch (Scenario C)
- `CollisionDetector` is **event-based only** — no polling
- `MainMenuMode` implements `IAppState` directly — NOT `IGameMode`
- All templates must have implementation **in header files**

---

*Last updated: Pre-development planning phase complete. Ready for CMakeLists.txt and coding.*
