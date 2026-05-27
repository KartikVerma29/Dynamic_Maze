#include "Maze.h"
#include <algorithm>
#include <memory>



Maze::Maze(int rows, int cols):rows(rows), cols(cols){
   for(int r=0; r<rows; r++){
      for(int c=0; c<cols; c++){
         cells.emplace(std::make_pair(r,c), Cell(r,c));
      }
   }

    for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
        // horizontal wall — between (r,c) and (r+1,c)
        if (r + 1 < rows) {
            auto wall = std::make_unique<Wall>(r, c, r+1, c, false);
            Wall* ptr = wall.get();
            walls.push_back(std::move(wall));
            getCell(r,   c)->setWall(DirectionType::DOWN, ptr);
            getCell(r+1, c)->setWall(DirectionType::UP,   ptr);
        }
        // vertical wall — between (r,c) and (r,c+1)
        if (c + 1 < cols) {
            auto wall = std::make_unique<Wall>(r, c, r, c+1, false);
            Wall* ptr = wall.get();
            walls.push_back(std::move(wall));
            getCell(r, c  )->setWall(DirectionType::RIGHT, ptr);
            getCell(r, c+1)->setWall(DirectionType::LEFT,  ptr);
        }
    }
}


}

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
   if(idx<0 || idx>=(int) walls.size()) return ;
   std::swap(walls[idx], walls.back());  
   walls.pop_back();
}

