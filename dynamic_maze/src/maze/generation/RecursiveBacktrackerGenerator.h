#pragma once
#include "IMazeGenerator.h"
#include <vector>
#include <utility>

class Wall;
class Cell;

class RecursiveBacktrackerGenerator : public IMazeGenerator{
private:
   std::vector<std::pair<int,int>> getUnvisit(Maze& maze, int row, int col);
   Wall* getWallbetween(Cell* cell, int row, int col, int nRow, int nCol);
public:
  void generate(Maze &maze) override;
};
