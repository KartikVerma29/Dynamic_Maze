#include "Maze.h"

Maze::Maze(int rows, int cols):rows(rows), cols(cols){
   cells.reserve(rows*cols);
   for(int r=0; r<rows; r++){
      for(int c=0; c<cols; c++){
         cells.emplace_back(r,c);
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

Cell* Maze::getCell(int r, int c){
   if(r<0 || c<0 ||r>=rows ||c>=cols ) return nullptr;
   Cell& cell = cells[r * cols + c];
   return cell.isActive()? &cell:nullptr;
}

const Cell* Maze::getCell(int r, int c) const{
   if(r<0 || c<0 ||r>=rows ||c>=cols ) return nullptr;
   const Cell& cell = cells[r * cols + c];
   return cell.isActive()? &cell:nullptr;
}

void Maze::setCell(int r, int c, Cell cell){
   if(r<0 || c<0 ||r>=rows ||c>=cols ) return;
   cells[r*cols+c] = cell;
}

void Maze::addWall(std::unique_ptr<Wall> wall){
   walls.push_back(std::move(wall));
}

void Maze::removeWall(const int idx){
   if(idx<0 || idx>=(int) walls.size()) return ;
   std::swap(walls[idx], walls.back());  
   walls.pop_back();
}

