#pragma once
#include "ISolvabilityChecker.h"
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <queue>
#include <vector>
#include "../Maze.h"


class Cell;

class BFSSolvabilityChecker:public ISolvabilityChecker{
private:
   std::vector<Position> reconstructPath(const std::vector<Cell*>& parent, Cell* start, Cell* exit, int cols);
public:
   bool isSolvable(Maze &maze,const Position &start,const Position &exit) override;
   std::vector<Position> getPath(Maze &maze,const Position &start,const Position &exit) override;
};
