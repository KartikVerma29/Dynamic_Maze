#pragma once
#include "IEnemy.h"
#include "pathfinding/IPathfinder.h"
#include "../player/Player.h"

class Chaser : public IEnemy
{
private:
   IPathfinder& pathfinder;
   // Position lastKnownPlayerPos;
   const Player& player;
   float moveTimer=0.0f;
   float moveInterval = 0.4f;
   float coolDown = 0.0f;
public:
   Chaser(IPathfinder& pathfinder, Position position, Direction direction, const Player& player);
   
   void update(Maze &maze, float deltaTime) override;
   void onEvent(const WallStateChangedEvent &event) override;

   EnemyType getType() const override{ return EnemyType::CHASER ;}

   bool isDefeatable(const Direction &direction) const override{
      return false;
   };
   
};
