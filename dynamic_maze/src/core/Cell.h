#pragma once
#include <unordered_map>
#include "Direction.h"
#include "../events/IEventListener.h"
#include "../events/events/WallAddedEvent.h"
#include "../events/events/WallStateChangedEvent.h"
#include "../events/events/WallRemovedEvent.h"
#include "Direction.h"
#include "Wall.h"

class Wall;

class Cell{
private:
   int row, col;
   std::unordered_map<DirectionType, Wall*> walls;
   bool visited, explored=false, active=true;
   DirectionType getDirection(int r, int c) const;
public:

   Cell():row(0), col(0), visited(false) {}
   Cell(int row, int col);

   Wall* getWall(const DirectionType& type);
   void setWall(const DirectionType& type, Wall* wall);
   bool hasOpenWall(DirectionType& type);

   int getRow() const;
   int getCol() const;
   bool isVisited() const;
   void setVisited(bool v);

   bool isExplored()const {return explored;}
   void setExplored(bool v){explored=v;}
   bool isActive()const {return active;}
   void setActive(bool v){active=v;}
};
