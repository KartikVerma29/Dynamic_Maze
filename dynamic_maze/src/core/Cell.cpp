#include "Cell.h"

Cell::Cell(int row, int col): row(row), col(col), visited(false) {}

Wall* Cell::getWall(DirectionType type) const{
   return walls[static_cast<size_t>(type)];
}

void Cell::setWall(DirectionType type, Wall* wall){
   walls[static_cast<size_t>(type)]=wall;
}

bool Cell::hasOpenWall(DirectionType type) const{
   Wall* w = walls[static_cast<size_t>(type)];
   return w && w->getIsOpen();
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

