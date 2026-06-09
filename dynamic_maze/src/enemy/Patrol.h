#pragma once
#include "pathfinding/IPathfinder.h"
#include "IEnemy.h"
#include "../events/events/PlayerMovedEvent.h"

class Patrol : public IEnemy, public IEventListener<PlayerMovedEvent>
{
private:
   IPathfinder& pathfinder;
   Direction lastPlayerDirection;

   Position diff = Position(
    cachedPath[pathIndex-1].getX() - position.getX(),
    cachedPath[pathIndex-1].getY() - position.getY()
);
   
public:
   Patrol(IPathfinder& pathfinder, Position position, Direction direction):
      IEnemy(position, direction), pathfinder(pathfinder) {
         lastPlayerDirection=DirectionType::DOWN;
      }

   EnemyType getType() const override{ return EnemyType::PATROL ;}
   void onEvent(const WallStateChangedEvent &event) override;
   void onEvent(const PlayerMovedEvent &event) override;
   void update(Maze &maze, float deltaTime) override;
   bool isDefeatable(const Direction &direction) const override;
};
