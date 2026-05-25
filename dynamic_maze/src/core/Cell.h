#pragma once
#include <unordered_map>
#include "Direction.h"
#include "../events/IEventListener.h"
#include "../events/events/WallAddedEvent.h"
#include "../events/events/WallStateChangedEvent.h"
#include "../events/events/WallRemovedEvent.h"

class Wall;

class Cell: public IEventListener<WallAddedEvent>, 
            public IEventListener<WallRemovedEvent>, 
            public IEventListener<WallStateChangedEvent> {
private:
   int row, col;
   std::unordered_map<DirectionType, Wall*> walls;
   bool visited;
   DirectionType getDirection(int r, int c) const;
   bool isRelevant(int r1, int c1, int r2, int c2, int& otherRow, int& otherCol) const;
public:

   Cell(int row, int col);

   Wall* getWall(DirectionType& type);
   void setWall(DirectionType& type, Wall* wall);
   bool hasOpenWall(DirectionType& type);

   int getRow() const;
   int getCol() const;
   bool isVisited() const;
   void setVisited(bool v);

   void onEvent(const WallAddedEvent& event) override;
   void onEvent(const WallRemovedEvent& evnt) override;
   void onEvent(const WallStateChangedEvent& evnt) override;
};
