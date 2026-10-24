#pragma once
#include "IMazeMutator.h"
#include "../solvability/ISolvabilityChecker.h"
#include "../../events/EventManager.h"
#include "../../events/events/WallTelegraphEvent.h"
#include <vector>
#include "../Maze.h"
#include <algorithm>
#include <random>
#include <cstdio>

class Wall;

class GridMazeMutator: public IMazeMutator{
private:
   static constexpr float MIN_MUTATE_DIST = 2.0f;
   static constexpr float MAX_MUTATE_DIST = 6.0f;
   static constexpr float DELAY_FACTOR = 0.15f; // Scale delay across distance
   static constexpr float MIN_DELAY = 0.1f; //Minimum warning duration for closest walls
   static constexpr float MAX_DELAY = 1.0f; // upper bound cap for whole ripple
   static constexpr float CLOSE_PROBABILITY = 0.95f;
   static constexpr float OPEN_PROBABILITY = 0.85f;

   std::mt19937 gen{std::random_device{}()};

   ISolvabilityChecker& checker;
   EventManager& eventManager;
   int batchSize;
   std::vector<std::pair<Wall*, float>> selectWalls(Maze& maze, const Position& playerPos, const Position& exitPos);

   struct PendingMutation{
      bool willOpen;
      float timeLeft;
      Wall* wall;
   };
   std::vector<PendingMutation> pendingQueue;

   void rollbackWall(Wall* wall);
   bool tryMutateWall(Maze& maze, Wall* wall, const Position& playerPos, const Position& exitPos);
   void queueWall(Wall* wall, float dist);
   std::vector<std::pair<Wall*,float>> applyWindow(std::vector<std::pair<Wall*,float>>& walls, int idx, Maze& maze);
   void publishChange(Wall* wall);
   bool canSafelyclose(Maze& maze, Wall* wall);
   bool createOpenBlock(Maze& maze, Wall* wall);
public:
   
   GridMazeMutator(ISolvabilityChecker& checker, EventManager& eventManager, int batchSize=3): checker(checker), eventManager(eventManager), batchSize(batchSize){}
   void mutate(Maze &maze,const Position &playerPos,const Position &exitPos) override;
   void update(float deltaTime, Maze& maze, const Position& playerPos, const Position& exitPos) override;
   void clearPending() override{pendingQueue.clear();}
   virtual ~GridMazeMutator() = default;
   
};
