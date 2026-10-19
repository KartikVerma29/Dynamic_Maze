#include "Chaser.h"

Chaser::Chaser(IPathfinder& pathfinder, Position position, Direction direction, const Player& player): 
IEnemy(position,direction,3.0f), pathfinder(pathfinder), player(player) {}

void Chaser::update(Maze& maze, float deltaTime){

   if(replanCooldown>0.0f) replanCooldown-=deltaTime;

   moveTimer+=deltaTime;

   if(moveTimer>=moveInterval){
      moveTimer=0.0f;
     
      bool hardTrigger = cachedPath.empty() || pathIndex>=(int) cachedPath.size();
      bool softTrigger = player.getPosition().distanceTo(cachedTarget)>DRIFT_THRESHOLD && replanCooldown<=0.0f;

      if(hardTrigger || softTrigger){
         cachedPath=pathfinder.findPath(maze, position, player.getPosition());
         cachedTarget=player.getPosition();
         replanCooldown=REPLAN_INTERVAL;
         pathIndex = (cachedPath.size()>1 && cachedPath[0]==position)?1:0;
      }
      tryStepNext(maze);
   }
}

