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

void IEnemy::stepTo(Position nextStep){
   if(nextStep==position) return ;
   
   int dx = nextStep.getX()-position.getX();
   int dy = nextStep.getY()-position.getY();

   if(dx==-1) direction=Direction(DirectionType::UP);
   else if (dx == 1) direction = Direction(DirectionType::DOWN);
   else if (dy == 1) direction = Direction(DirectionType::RIGHT);
   else if (dy == -1) direction = Direction(DirectionType::LEFT);

   position = nextStep;
   visual.setTag(position);

}

void IEnemy::onEvent(const WallStateChangedEvent &event){
   if(cachedPath.empty() || event.isOpen) return;
   if(isWallOnPath(event)){
      cachedPath.clear();
      pathIndex=0;
   }
}

bool IEnemy::tryStepNext(Maze &maze){
   if(pathIndex>=(int)cachedPath.size()) return false;
   Position nextStep = cachedPath[pathIndex];
   if(nextStep == position){
      pathIndex++;
      if(pathIndex>=(int)cachedPath.size()) return false;
      nextStep = cachedPath[pathIndex];
   }
   int dx = (int)nextStep.getX()-(int)position.getX();
   int dy = (int)nextStep.getY()-(int)position.getY();

   DirectionType dir;
   if (dx == -1 && dy == 0) dir = DirectionType::UP;
   else if (dx == 1 && dy == 0) dir = DirectionType::DOWN;
   else if (dx == 0 && dy == 1) dir = DirectionType::RIGHT;
   else if (dx == 0 && dy == -1) dir = DirectionType::LEFT;
   else{
      cachedPath.clear();
      pathIndex=0;
      return false;
   }

   Cell* curr = maze.getCell((int)position.getX(), (int)position.getY());
   if(!curr || !curr->hasOpenWall(dir)){
      cachedPath.clear();
      pathIndex=0;
      return false;
   }

   pathIndex++;
   stepTo(nextStep);
   return true;
}