#pragma once
#include "IEnemy.h"
#include "pathfinding/IPathfinder.h"
#include "../events/events/PlayerMovedEvent.h"

class Chaser : public IEnemy, public IEventListener<PlayerMovedEvent>
{
private:
   IPathfinder& pathfinder;
   Position lastKnownPlayerPos;
public:
   Chaser(IPathfinder& pathfinder, Position position, Direction direction);
   
   void update(Maze &maze, float deltaTime) override;
   void onEvent(const WallStateChangedEvent &event) override;
   void onEvent(const PlayerMovedEvent &event) override;

   EnemyType getType() const override{ return EnemyType::CHASER ;}

   bool isDefeatable(const Direction &direction) const override{
      return false;
   };
   
};
