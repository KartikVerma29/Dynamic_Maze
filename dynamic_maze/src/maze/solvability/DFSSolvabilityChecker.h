#pragma once
#include "ISolvabilityChecker.h"

class DFSSolvabilityChecker: public ISolvabilityChecker{
public:
   bool isSolvable(Maze &maze,const Position &start,const Position &exit) override;
   std::vector<Position> getPath(Maze &maze,const Position &start,const Position &exit) override;
};
//need to complete implementation