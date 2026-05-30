#pragma once
#include "IEnemy.h"
#include "pathfinding/IPathfinder.h"

class Chaser : public IEnemy
{
private:
   IPathfinder& pathfinder;
public:
   Chaser(IPathfinder& pathfinder, Position position, Direction direction);
   void update(Maze &maze, float deltaTime) override;
   bool isDefeatable(const Direction &direction) const override;
   
};
