#include "Patrol.h"
#include "pathfinding/IPathfinder.h"

Patrol::Patrol(std::vector<Position> route,IPathfinder& finder ,Position position, Direction direction):
    IEnemy(position, direction,2.0f), pathfinder(route,finder){
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
   return !(this->direction == direction.opposite());
}

void Patrol::update(Maze& maze, float deltaTime) {
    if (isDefeated) return;

    moveTimer+=deltaTime;
    if(moveTimer>=moveInterval){
        moveTimer=0.0f;
        int n = (int)cachedPath.size();
        if(cachedPath.empty() || pathIndex>=n){
            cachedPath = pathfinder.findPath(maze, position, position);
            pathIndex = 0;
        }
        if(!cachedPath.empty()&& pathIndex<n){
           stepTo(cachedPath[pathIndex++]);
        }
    }
    
}
