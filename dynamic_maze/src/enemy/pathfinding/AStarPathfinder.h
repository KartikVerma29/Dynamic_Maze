#pragma once
#include "IPathfinder.h"
#include <vector>

class AStarPathfinder: public IPathfinder
{
public:
   std::vector<Position> findPath(Maze &maze, Position &from,const Position &to) override;
   
   ~AStarPathfinder() = default;
};

