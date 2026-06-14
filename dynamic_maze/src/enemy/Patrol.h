#pragma once
#include "pathfinding/PatrolPathfinder.h"
#include "IEnemy.h"
#include "../events/events/PlayerMovedEvent.h"

class Patrol : public IEnemy, public IEventListener<PlayerMovedEvent>
{
private:
   PatrolPathfinder pathfinder;
   Direction lastPlayerDirection;

   float moveTimer=0.0f, moveInterval=0.6f ;

public:
   Patrol(std::vector<Position> route, Position position, Direction direction);

   EnemyType getType() const override{ return EnemyType::PATROL ;}
   void onEvent(const WallStateChangedEvent &event) override;
   void onEvent(const PlayerMovedEvent &event) override;
   void update(Maze &maze, float deltaTime) override;
   bool isDefeatable(const Direction &direction) const override;
};
