#include "Blocker.h"

void Blocker::onEvent(const WallStateChangedEvent& event){
   if(cachedPath.empty()) return ;
   if(event.isOpen) return;
   if(isWallOnPath(event)){
      cachedPath.clear();
      pathIndex=0;
   }
}

Position Blocker::findCorridorTarget(Maze& maze)const{
   Cell* current = maze.getCell((int)position.getX(), (int)position.getY());

   Position best = position;
   int bestscore = 5;

   for(auto dir : {DirectionType::UP, DirectionType::DOWN, DirectionType::LEFT, DirectionType::RIGHT}){
      if(!current->hasOpenWall(dir)) continue;
      
      Direction d(dir);
      Position offset = d.toVector();
      int nr = position.getX()+offset.getX();
      int nc = position.getY()+offset.getY();

      Cell* neighbour = maze.getCell(nr,nc);
      if(!neighbour) continue;

      int openCount=0;
      for(auto d2 : {DirectionType::UP, DirectionType::DOWN, DirectionType::RIGHT, DirectionType::LEFT}){
         if(!neighbour->hasOpenWall(d2)) openCount++;
      }

      if(openCount<bestscore){
         bestscore=openCount;
         best=Position(nr,nc);
      }
   }
   return best;
}

void Blocker::update(Maze& maze, float deltaTime){
   if(!isMoving){
      idleTimer-=deltaTime;
      if(idleTimer<=0.0f){
         isMoving=true;
         stepsRemaining=3;
      }
   } else {
      if(stepsRemaining<=0){
         isMoving=false;
         idleTimer=3.0f;
         return;
      }

      position = findCorridorTarget(maze);
      stepsRemaining--;
   }
}
