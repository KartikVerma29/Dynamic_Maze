#include "Chaser.h"

Chaser::Chaser(IPathfinder& pathfinder, Position position, Direction direction): IEnemy(position,direction), pathfinder(pathfinder) {}

void Chaser::onEvent(const WallStateChangedEvent& event){
   if(cachedPath.empty()) return;
   if(event.isOpen) return ;
   if(isWallOnPath(event)){
      cachedPath.clear();
      pathIndex=0;
   }
}

void Chaser::onEvent(const PlayerMovedEvent& event){
   lastKnownPlayerPos = event.newPosition;
   cachedPath.clear();
}

void Chaser::update(Maze& maze, float deltaTime){
   if(cachedPath.empty() || pathIndex>=cachedPath.size()){
      cachedPath = pathfinder.findPath(maze, position, lastKnownPlayerPos);
      pathIndex=0;
   }
   if(pathIndex<cachedPath.size()) position=cachedPath[pathIndex++];
}

