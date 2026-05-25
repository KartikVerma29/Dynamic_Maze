#include "Maze.h"
#include <algorithm>
#include <memory>

Cell* Maze::getCell(int row, int col){
   if(row<0 || col<0 || rows<=row || cols<=col ) return nullptr;
   auto it = cells.find({row, col});
   if(it==cells.end()) return nullptr;
   return &it->second;
}

const Cell* Maze::getCell(int row, int col) const{
   if(row<0 || col<0 || rows<=row || cols<=col) return nullptr;
   auto it=cells.find({row,col});
   if(it==cells.end()) return nullptr;
   return &it->second;
}

void Maze::setCell(int row, int col, Cell cell){
   if( rows<row || cols<col) return ;
   cells[{row, col}] = cell;
}

void Maze::addWall(std::unique_ptr<Wall> wall){
   walls.push_back(std::move(wall));
}

void Maze::removeWall(const int idx){
   if(idx<0 || idx>=walls.size()) return ;
   std::swap(walls[idx], walls.back());  
   walls.pop_back();
}

