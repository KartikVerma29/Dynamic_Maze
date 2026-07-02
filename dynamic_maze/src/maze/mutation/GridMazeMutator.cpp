#include "GridMazeMutator.h"
#include "IMazeMutator.h"
#include "../Maze.h"
#include <algorithm>

std::vector<std::pair<Wall*,float>> GridMazeMutator::selectWalls(Maze& maze, const Position& playerPos){
   std::vector<std::pair<Wall*,float>> result;
   
   float scaleFactor = maze.getRows()/15.0f;

   float minThreshold = 1.0f;
   float maxThreshold = 6.0f;

   for(const auto& w:maze.getWalls()){
      Wall* wall = w.get();

      auto[r1,c1] = wall->getCells().first;
      auto[r2,c2] = wall->getCells().second;

      Position mid( (r1+r2)/2.0f, (c1+c2)/2.0f );
      float d = playerPos.distanceTo(mid);

      if(d>= minThreshold && d<maxThreshold)
         result.push_back({wall,d});
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
   auto selected = selectWalls(maze,playerPos);
   if(selected.empty()) return;
   std::sort(selected.begin(), selected.end(), [](const auto& a, const auto&b){
      return a.second > b.second;
   });

   float scaleFactor = maze.getRows()/15.0f;
   float safezone = 2.0f;

   for(const auto& pair:selected){
      Wall* w = pair.first;
      float dist = pair.second;

      if(!w->getIsOpen() && dist<=safezone) continue;

      bool chanage = tryMutateWall(maze, w, playerPos, exitPos);
   }

}
