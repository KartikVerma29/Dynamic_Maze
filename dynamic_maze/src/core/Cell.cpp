#include "Cell.h"
#include "Direction.h"
#include "Wall.h"

Cell::Cell(int row, int col): row(row), col(col), visited(false) {}

Wall* Cell::getWall(const DirectionType& type){
   auto it=walls.find(type);
   if(it==walls.end()) return nullptr;
   return it->second;
}

void Cell::setWall(const DirectionType& type, Wall* wall){
   walls[type] = wall;
}

bool Cell::hasOpenWall(DirectionType& type){
   auto it=walls.find(type);
   if(it==walls.end()) return false;
   return it->second->getIsOpen();
}

int Cell::getRow() const{ return this->row;}
int Cell::getCol() const{ return this->col;}
bool Cell::isVisited() const{ return this->visited;}
void Cell::setVisited(bool v){ this->visited=v;}

DirectionType Cell::getDirection(int oRow, int oCol) const{
   if(oRow<row) return DirectionType::UP;
   if(oRow>row) return DirectionType::DOWN;
   if(oCol<col) return DirectionType::LEFT;
   return DirectionType::RIGHT;
}

bool Cell::isRelevant(int r1, int c1, int r2, int c2, int& otherRow, int& otherCol) const {
        if (r1 == row && c1 == col) { otherRow = r2; otherCol = c2; return true; }
        if (r2 == row && c2 == col) { otherRow = r1; otherCol = c1; return true; }
        return false;
}


void Cell::onEvent(const WallAddedEvent& event){
   int oRow=0, oCol=0;
   if(isRelevant(event.row1, event.col1, event.row2, event.col2, oRow, oCol))
      walls[getDirection(oRow, oCol)] = event.wall;
}

void Cell::onEvent(const WallRemovedEvent& event){
   int oRow=0, oCol=0;
   if(isRelevant(event.row1, event.col1, event.row2, event.col2, oRow, oCol))
      walls.erase(getDirection(oRow, oCol));
}

void Cell::onEvent(const WallStateChangedEvent& event){
  int oRow=0, oCol=0;
  if(isRelevant(event.row1, event.col1 , event.row2, event.col2, oRow, oCol)){
     auto it=walls.find(getDirection(oRow, oCol));
       if(it==walls.end()) return;
     Wall* wall = it->second;
     
     if(wall){
        if(event.isOpen) wall->open();
        else wall->close();
     }
  }
}
