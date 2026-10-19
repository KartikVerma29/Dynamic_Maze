#pragma once
#include "pathfinding/IPathfinder.h"
#include "pathfinding/PatrolPathfinder.h"
#include "IEnemy.h"

class Patrol : public IEnemy{
private:
   PatrolPathfinder pathfinder;

   float moveTimer=0.0f, moveInterval=0.6f ;

public:
   Patrol(std::vector<Position> route,IPathfinder& finder ,Position position, Direction direction);

   EnemyType getType() const override{ return EnemyType::PATROL ;}
   void update(Maze &maze, float deltaTime) override;
   bool isDefeatable(const Direction &direction) const override;
};
