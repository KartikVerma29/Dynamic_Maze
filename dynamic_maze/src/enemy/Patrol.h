#pragma once
#include "pathfinding/IPathfinder.h"
#include "IEnemy.h"

class Patrol : public IEnemy
{
public:
   IPathfinder& pathfinder;
   Direction facing;
private:

   Patrol(IPathfinder& pathfinder, Position position, Direction direction);

   void update(Maze &maze, float deltaTime) override;
   bool isDefeatable(const Direction &direction) const override;
};
