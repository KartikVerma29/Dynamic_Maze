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

// void Chaser::onEvent(const PlayerMovedEvent& event){
//    lastKnownPlayerPos = event.newPosition;
//    cachedPath.clear();
// }

void Chaser::update(Maze& maze, float deltaTime){
   moveTimer+=deltaTime;

   if(moveTimer>=moveInterval){
      moveTimer=0.0f;
      if(cachedPath.empty() || pathIndex>=cachedPath.size()){
         cachedPath = pathfinder.findPath(maze, position, player.getPosition());
         pathIndex=0;
      }
      if(!cachedPath.empty() && pathIndex<cachedPath.size()) position=cachedPath[pathIndex++];
   }
}

