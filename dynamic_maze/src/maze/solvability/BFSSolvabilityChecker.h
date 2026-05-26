#pragma once
#include "ISolvabilityChecker.h"
#include <unordered_map>


class Cell;

class BFSSolvabilityChecker:public ISolvabilityChecker{
private:
   std::vector<Position> reconstructPath(std::unordered_map<Cell*, Cell*>& parent, Cell* start, Cell* exit);
public:
   bool isSolvable(Maze &maze,const Position &start,const Position &exit) override;
   std::vector<Position> getPath(Maze &maze,const Position &start,const Position &exit) override;
};
