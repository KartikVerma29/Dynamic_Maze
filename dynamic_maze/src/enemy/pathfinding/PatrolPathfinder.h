#pragma once
#include "IPathfinder.h"
#include <vector>

class PatrolPathfinder:public IPathfinder{
private:
   std::vector<Position> route;
   IPathfinder& finder;
   int currentTarget = 0;
public:

   PatrolPathfinder(std::vector<Position> route,IPathfinder& finder ,int currentTarget=0);

   std::vector<Position> findPath(Maze &maze, Position &from,const Position &to) override;
   ~PatrolPathfinder() = default;
};
