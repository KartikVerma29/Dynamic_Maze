#include "GridMazeMutator.h"
#include "IMazeMutator.h"
#include "../Maze.h"
#include <algorithm>
#include <tuple>




std::vector<Wall*> GridMazeMutator::selectWalls(Maze& maze, const Position& playerPos){
   std::vector<Wall*> result;
   
   for(const auto&w : maze.getWalls()){
      Wall* wall = w.get();
      result.push_back(wall);
   }

   int cnt = std::min(batchSize, (int)result.size());

   std::nth_element(result.begin(), result.begin()+cnt, result.end(),
         [&playerPos](const Wall* a, const Wall* b){
   
            auto [ar1, ac1, ar2, ac2] = std::make_tuple(a->getCells().first.first, a->getCells().first.second, a->getCells().second.first, a->getCells().second.second);
            auto[br1, bc1, br2, bc2]=   std::make_tuple(b->getCells().first.first, b->getCells().first.second, b->getCells().second.first, b->getCells().second.second);
           
            Position aMid((ar1+ar2)/2.0f, (ac1+ac2)/2.0f);
            Position bMid((br1+br2)/2.0f, (bc1+bc2)/2.0f);
         return playerPos.distanceTo(aMid) < playerPos.distanceTo(bMid);
   });
   
   result.resize(cnt);
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
