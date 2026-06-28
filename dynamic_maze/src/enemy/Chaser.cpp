#include "Chaser.h"

Chaser::Chaser(IPathfinder& pathfinder, Position position, Direction direction, const Player& player): 
IEnemy(position,direction), pathfinder(pathfinder), player(player) {}

void Chaser::onEvent(const WallStateChangedEvent& event){
   if(cachedPath.empty()) return;
   if(event.isOpen) return ;
   if(isWallOnPath(event)){
      cachedPath.clear();
      pathIndex=0;
   }
}

void Chaser::update(Maze& maze, float deltaTime){

   if(coolDown>0.0f) coolDown-=deltaTime;

   moveTimer+=deltaTime;

   if(moveTimer>=moveInterval){
      moveTimer=0.0f;
      if(cachedPath.empty() || pathIndex>=cachedPath.size()){
         if(coolDown<=0.0f){
            cachedPath = pathfinder.findPath(maze, position, player.getPosition());
            pathIndex=0;

            if(cachedPath.empty()) coolDown=0.5f;
            else if (cachedPath[0]==position) pathIndex=1;

         }
      }
      if(!cachedPath.empty() && pathIndex<cachedPath.size()) position=cachedPath[pathIndex++];
   }
}

