#include "IEnemy.h"

bool IEnemy::isWallOnPath(const WallStateChangedEvent& event)const{
   int startIndex = std::max(0, pathIndex-1);
    for(int i=startIndex; i+1<(int)cachedPath.size(); i++){
      const Position& curr = cachedPath[i];
      const Position& next = cachedPath[i+1];


      bool crosses = (
               ((curr.getX() == event.row1 && curr.getY()==event.col1) && 
               (next.getX() == event.row2 && next.getY()==event.col2)) 
               ||
               ((curr.getX() == event.row2 && curr.getY()==event.col2) && 
               (next.getX() == event.row1 && next.getY()==event.col1))
            );
      if(crosses) return true;
   }

   return false;
}