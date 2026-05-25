#pragma once
#include "ISolvabilityChecker.h"

class DFSSolvabilityChecker: public ISolvabilityChecker{
public:
   bool isSolvable(Maze &maze, Position &start, Position &exit) override;
   std::vector<Position> getPath(Maze &maze, Position &start, Position &exit) override;
};
