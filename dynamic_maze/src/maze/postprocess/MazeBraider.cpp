#include "MazeBraider.h"
#include "../../core/Direction.h"
#include "../../core/Wall.h"
#include "../../core/Position.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <random>

int MazeBraider::openCount(Cell *cell) const{
   int cntWall=0;
   for(auto dir:ALL_DIRECTIONS){
      if(cell->hasOpenWall(dir)) cntWall++;
   }
   return cntWall;
}

bool MazeBraider::isDeadEnd(Cell *cell) const{
   return 1==openCount(cell);    
}

bool MazeBraider::createsOpenBlock(Maze &maze, Cell *cell, DirectionType dir) const{
   Position off = Direction(dir).toVector();
   Cell* curr = maze.getCell(cell->getRow()+off.getX(), cell->getCol()+off.getY());
   if(!curr) return false;

   Direction side = Direction(dir).rotate90();
   for(DirectionType p:{side.getType(), side.opposite().getType()}){
      if(!cell->hasOpenWall(p) || !curr->hasOpenWall(p)) continue;
      Position po = Direction(p).toVector();

      Cell* ap = maze.getCell(cell->getRow()+po.getX(), cell->getCol()+po.getY());
      if(ap && ap->hasOpenWall(dir)) return true;
   }
   return false;
}

Wall* MazeBraider::pickClosedNeighbour(Maze &maze, Cell *cell){
   std::vector<Wall*> deadEndWalls, otherWalls;
   for(auto dir:ALL_DIRECTIONS){
      Wall* wall = cell->getWall(dir);
      if(!wall || wall->getIsOpen()) continue;

      Position off = Direction(dir).toVector();
      Cell* neighbour = maze.getCell(cell->getRow()+off.getX(), cell->getCol()+off.getY());
      if(!neighbour) continue;
      if(4-(openCount(neighbour)+1)<minWalls) continue;
      if(createsOpenBlock(maze, cell, dir)) continue;

      (isDeadEnd(neighbour)? deadEndWalls:otherWalls).push_back(wall);
   }

   auto& pool = !deadEndWalls.empty()? deadEndWalls:otherWalls;
   if(pool.empty()) return nullptr;

   std::uniform_int_distribution<size_t> pick(0, pool.size()-1);
   return pool[pick(rng)];
}

void MazeBraider::process(Maze &maze){

   int rows = maze.getRows();
   int cols = maze.getCols();

   std::vector<Cell*> deadEnds; 

   for(int r=0; r<rows; r++){
      for(int c=0; c<cols; c++){
         Cell* curr = maze.getCell(r,c);
         if(curr && isDeadEnd(curr)) deadEnds.push_back(curr);
      }
   }
   std::cerr<<"Factor: "<<braidFactor;
   std::cerr<<"\nDeadEnds before: "<<deadEnds.size();
   std::shuffle(deadEnds.begin(), deadEnds.end(), rng);

   std::bernoulli_distribution coin(braidFactor);

   for(Cell* it:deadEnds){
      if(isDeadEnd(it) && coin(rng)){
        if(Wall* wall = pickClosedNeighbour(maze, it)) wall->open();
      }
   }

   int cntDeadEnd=0;
   for(int r=0; r<rows; r++){
      for(int c=0; c<cols; c++){
         Cell* curr = maze.getCell(r,c);
         if(curr && isDeadEnd(curr)) cntDeadEnd++;}
   }
   std::cerr<<"\nDeadEnds after: "<<cntDeadEnd;

}
