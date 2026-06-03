#pragma once
#include "IPathfinder.h"
#include "AStarPathfinder.h"
#include <vector>

class PatrolPathfinder:public IPathfinder{
private:
   std::vector<Position> route;
   int currentTarget = 0;
   AStarPathfinder astar;
public:

   PatrolPathfinder(std::vector<Position> route, int currentTarget=0);

   std::vector<Position> findPath(Maze &maze, Position &from, Position &to) override;
   ~PatrolPathfinder() = default;
};
