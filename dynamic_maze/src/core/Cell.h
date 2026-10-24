#pragma once
#include <array>
#include "Direction.h"
#include "Wall.h"

class Wall;

class Cell{
private:
   int row, col;
   std::array<Wall*,4> walls{nullptr,nullptr,nullptr,nullptr};
   bool visited=false, explored=false, active=true;
   DirectionType getDirection(int r, int c) const;
public:

   Cell():row(0), col(0), visited(false) {}
   Cell(int row, int col);

   Wall* getWall(DirectionType type) const;
   void setWall(DirectionType type, Wall* wall);
   bool hasOpenWall(DirectionType type) const;

   int getRow() const;
   int getCol() const;
   bool isVisited() const;
   void setVisited(bool v);

   bool isExplored()const {return explored;}
   void setExplored(bool v){explored=v;}
   bool isActive()const {return active;}
   void setActive(bool v){active=v;}
};
