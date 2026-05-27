#pragma once
#include "PairHash.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include "../core/Cell.h"
#include "../core/Wall.h"

class Maze{
private:
   std::unordered_map<std::pair<int,int>, Cell, PairHash> cells;
   std::vector<std::unique_ptr<Wall>> walls;
   int rows, cols;

public:
   Maze(int rows, int cols);

   Cell* getCell(int row, int col);
   const Cell* getCell(int row, int col) const;

   void setCell(int row, int col, Cell cell);
   void addWall(std::unique_ptr<Wall> wall);
   void removeWall(const int idx);
   const std::vector<std::unique_ptr<Wall>>& getWalls() const { return walls;}

   int getRows() const{ return rows;}
   int getCols() const{ return cols;}
};
