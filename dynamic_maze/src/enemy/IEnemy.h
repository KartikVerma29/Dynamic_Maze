#pragma once
#include "../maze/Maze.h"
#include "../events/events/WallStateChangedEvent.h"
#include "../events/IEventListener.h"
#include "../core/Direction.h"
#include "../core/Position.h"

enum class EnemyType{CHASER, PATROL, BLOCKER};

class IEnemy:public IEventListener<WallStateChangedEvent>{
protected:
   Position position;
   Direction direction;
   std::vector<Position> cachedPath;
   int pathIndex=0;
   bool isDefeated=false;
   
   bool isWallOnPath(const WallStateChangedEvent& event) const;
   
   public:
   IEnemy(Position position, Direction direction) : position(position), direction(direction){}
   
   virtual void update(Maze& maze, float deltaTime) = 0;
   virtual bool isDefeatable(const Direction& direction) const = 0;
   virtual void onEvent(const WallStateChangedEvent &event) = 0;
   virtual EnemyType getType()const =0;
   Position getPosition() const{
      return position;
   };
   bool getIsDefeated() const{
      return isDefeated;
   };
   void setDefeated(){
      isDefeated=true;
   }
   
   virtual ~IEnemy() = default;
};
