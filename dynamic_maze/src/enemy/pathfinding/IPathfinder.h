#pragma once
#include "../../maze/Maze.h"

class IPathfinder{
public:
   virtual std::vector<Position> findPath(Maze& maze, Position& from,const Position& to) = 0;
};
