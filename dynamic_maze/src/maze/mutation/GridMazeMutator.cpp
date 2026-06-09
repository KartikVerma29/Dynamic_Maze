#include "GridMazeMutator.h"
#include "IMazeMutator.h"
#include "../Maze.h"
#include <algorithm>
#include <tuple>

std::vector<Wall*> GridMazeMutator::selectWalls(Maze& maze, const Position& playerPos){
   std::vector<Wall*> result;
   
   float minThreshold = 1.0f;
   float maxThreshold = 2.0f;

   for(const auto& w:maze.getWalls()){
      Wall* wall = w.get();

      auto[r1,c1] = wall->getCells().first;
      auto[r2,c2] = wall->getCells().second;

      Position mid( (r1+c1)/2.0f, (r2+c2)/2.0f );
      float d = playerPos.distanceTo(mid);

      if(d>= minThreshold && d<maxThreshold)
         result.push_back(wall);
   }
   return result;
}

bool GridMazeMutator::tryMutateWall(Maze& maze, Wall* wall, const Position& playerPos, const Position& exitPos){
   if(wall->getIsOpen()) wall->close();
   else wall->open();

   if(checker.isSolvable(maze, playerPos, exitPos)){
      auto cell = wall->getCells();
      eventManager.publish<WallStateChangedEvent>({
               cell.first.first, cell.first.second,
               cell.second.first, cell.second.second, wall->getIsOpen()
            });
      return true;
   }
   rollbackWall(wall);
   return false;
}

void GridMazeMutator::rollbackWall(Wall* wall){
   if(wall->getIsOpen()) wall->close();
   else wall->open();
}

void GridMazeMutator::mutate(Maze& maze,const Position& playerPos,const Position& exitPos){
   std::vector<Wall*> selected = selectWalls(maze, playerPos);
   if(selected.empty()) return;

   for(Wall* w:selected){
      bool change = tryMutateWall(maze, w, playerPos, exitPos);
   }

}
