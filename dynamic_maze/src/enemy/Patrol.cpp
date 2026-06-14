#include "Patrol.h"

Patrol::Patrol(std::vector<Position> route, Position position, Direction direction):
    IEnemy(position, direction), pathfinder(route){
        lastPlayerDirection = DirectionType::DOWN;
    }

void Patrol::onEvent(const WallStateChangedEvent& event){
   if(cachedPath.empty()) return;
   if(event.isOpen) return;
   if(isWallOnPath(event)){
      cachedPath.clear();
      pathIndex=0;
   }
}

void Patrol::onEvent(const PlayerMovedEvent& event){
   lastPlayerDirection = event.direction;
}

bool Patrol::isDefeatable(const Direction& direction)const{
   return lastPlayerDirection==direction;
}

void Patrol::update(Maze& maze, float deltaTime) {
    if (isDefeated) return;

    moveTimer+=deltaTime;
    if(moveTimer>=moveInterval){
        moveTimer=0.0f;
        if(cachedPath.empty() || pathIndex>=cachedPath.size()){
            cachedPath = pathfinder.findPath(maze, position, position);
            pathIndex = 0;
        }
        if(!cachedPath.empty()&& pathIndex<cachedPath.size()){
            position=cachedPath[pathIndex++];
        }
    }
    
}
