#pragma once
#include "IMazeMutator.h"
#include "../solvability/ISolvabilityChecker.h"
#include "../../events/EventManager.h"
#include <vector>

class Wall;

class GridMazeMutator: public IMazeMutator{
private:
ISolvabilityChecker& checker;
EventManager& eventManager;
int batchSize;

   std::vector<std::pair<Wall*, float>> selectWalls(Maze& maze, const Position& playerPos);
   bool tryMutateWall(Maze& maze, Wall* wall, const Position& playerPos, const Position& exitPos);
   void rollbackWall(Wall* wall);
public:

   GridMazeMutator(ISolvabilityChecker& checker, EventManager& eventManager, int batchSize=3): checker(checker), eventManager(eventManager), batchSize(batchSize){}

   void mutate(Maze &maze,const Position &playerPos,const Position &exitPos) override;
   virtual ~GridMazeMutator() = default;
   
};
