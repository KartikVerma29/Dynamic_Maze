#include "Patrol.h"
#include "pathfinding/IPathfinder.h"

Patrol::Patrol(std::vector<Position> route,IPathfinder& finder ,Position position, Direction direction):
    IEnemy(position, direction,2.0f), pathfinder(route,finder){}

bool Patrol::isDefeatable(const Direction& direction)const{
   return !(this->direction == direction.opposite());
}

void Patrol::update(Maze& maze, float deltaTime) {
    if (isDefeated) return;

    moveTimer+=deltaTime;
    if(moveTimer>=moveInterval){
        moveTimer=0.0f;
        if(cachedPath.empty() || pathIndex>=(int)cachedPath.size()){
            cachedPath = pathfinder.findPath(maze,position,position);
            pathIndex=(cachedPath.size()>1 && cachedPath[0]==position)?1:0;
        }
        tryStepNext(maze);
    }
}
