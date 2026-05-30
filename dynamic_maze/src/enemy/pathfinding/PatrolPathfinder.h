#pragma once
#include "IPathfinder.h"

class PatrolPathfinder:public IPathfinder{
private:
   std::vector<Position> route;
public:
   std::vector<Position> findPath(Maze &maze, Position &from, Position &to) const override;
};
