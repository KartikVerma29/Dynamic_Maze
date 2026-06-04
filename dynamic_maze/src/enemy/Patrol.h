#pragma once
#include "pathfinding/IPathfinder.h"
#include "IEnemy.h"
#include "../events/events/PlayerMovedEvent.h"

class Patrol : public IEnemy, public IEventListener<PlayerMovedEvent>
{
public:
   IPathfinder& pathfinder;
   Direction facing;
   Direction lastPlayerDirection;
   
private:
   EnemyType getType() const override{ return EnemyType::PATROL ;}
   void onEvent(const WallStateChangedEvent &event) override;
   void update(Maze &maze, float deltaTime) override;
   bool isDefeatable(const Direction &direction) const override;
};
