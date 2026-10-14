#include "Patrol.h"
#include "pathfinding/IPathfinder.h"

Patrol::Patrol(std::vector<Position> route,IPathfinder& finder ,Position position, Direction direction):
    IEnemy(position, direction,2.0f), pathfinder(route,finder){}

void Patrol::onEvent(const WallStateChangedEvent& event){
   if(cachedPath.empty()) return;
   if(event.isOpen) return;
   if(isWallOnPath(event)){
      cachedPath.clear();
      pathIndex=0;
   }
}

bool Patrol::isDefeatable(const Direction& direction)const{
   return !(this->direction == direction.opposite());
}

void Patrol::update(Maze& maze, float deltaTime) {
    if (isDefeated) return;

    moveTimer+=deltaTime;
    if(moveTimer>=moveInterval){
        moveTimer=0.0f;
        if(cachedPath.empty() || pathIndex>=(int)cachedPath.size()){
            cachedPath = pathfinder.findPath(maze, position, position);
            pathIndex = 0;
        }
        if(!cachedPath.empty()&& pathIndex<(int)cachedPath.size()){
           stepTo(cachedPath[pathIndex++]);
        }
    }
    
}
