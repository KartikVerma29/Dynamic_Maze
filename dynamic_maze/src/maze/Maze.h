#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include "../core/Cell.h"
#include "../core/Wall.h"

class Maze{
private:
   std::vector<Cell> cells;
   std::vector<std::unique_ptr<Wall>> walls;
   int rows, cols;

public:
   Maze(int rows, int cols);

   Cell* getCell(int r, int c);
   const Cell* getCell(int r, int c) const;

   void setCell(int r, int c, Cell cell);
   void addWall(std::unique_ptr<Wall> wall);
   void removeWall(const int idx);
   const std::vector<std::unique_ptr<Wall>>& getWalls() const { return walls;}

   int getRows() const{ return rows;}
   int getCols() const{ return cols;}
};
