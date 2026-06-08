#include "Patrol.h"

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

