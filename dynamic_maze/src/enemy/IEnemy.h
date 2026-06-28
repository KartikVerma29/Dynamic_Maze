#pragma once
#include "../maze/Maze.h"
#include "../events/events/WallStateChangedEvent.h"
#include "../events/IEventListener.h"
#include "../core/Direction.h"
#include "../core/Position.h"
#include "../core/VisualComponent.h"

enum class EnemyType{CHASER, PATROL, BLOCKER};

class IEnemy:public IEventListener<WallStateChangedEvent>{
protected:
   VisualComponent visual;
   Position position;
   Direction direction;
   std::vector<Position> cachedPath;
   int pathIndex=0;
   bool isDefeated=false;
   bool isWallOnPath(const WallStateChangedEvent& event) const;
   void stepTo(Position nextStep);
   
public:
   IEnemy(Position position, Direction direction, float visualSpeed) : position(position), direction(direction), visual(position,visualSpeed){}
   
   virtual void update(Maze& maze, float deltaTime) = 0;
   virtual bool isDefeatable(const Direction& direction) const = 0;
   virtual void onEvent(const WallStateChangedEvent &event) = 0;
   virtual EnemyType getType()const =0;
   Position getPosition() const{
      return position;
   };
   Direction getDirection() const{
      return direction;
   }
   bool getIsDefeated() const{
      return isDefeated;
   };
   void setDefeated(){
      isDefeated=true;
   }
   
   void updateVisual(float deltaTime) { visual.update(deltaTime); }
   FloatPos getRenderPosition() const { return visual.getRenderPosition();}

   virtual ~IEnemy() = default;
};
