#pragma once
#include "../maze/Maze.h"
#include "../events/events/WallStateChangedEvent.h"
#include "../events/IEventListener.h"
#include "../core/Direction.h"
#include "../core/Position.h"

class IEnemy:public IEventListener<WallStateChangedEvent>{
protected:
   Position position;
   Direction direction;
   bool isDefeated=false;
public:
   

   virtual void update(Maze& maze, float deltaTime) = 0;
   virtual bool isDefeatable(const Direction& direction) const = 0;
   void onEvent(const WallStateChangedEvent &event) override;

   Position getPosition() const{
      return position;
   };
   bool getIsDefeated() const{
      return isDefeated;
   };
   
   virtual ~IEnemy() = default;
};
