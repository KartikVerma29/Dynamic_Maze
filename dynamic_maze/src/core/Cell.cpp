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

