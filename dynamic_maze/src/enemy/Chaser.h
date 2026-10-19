#pragma once
#include "IEnemy.h"
#include "pathfinding/IPathfinder.h"
#include "../player/Player.h"

static constexpr float REPLAN_INTERVAL=0.5f;
static constexpr float DRIFT_THRESHOLD=3.0f;

class Chaser : public IEnemy
{
private:
   IPathfinder& pathfinder;
   // Position lastKnownPlayerPos;
   const Player& player;
   float moveTimer=0.0f;
   float moveInterval = 0.4f;

public:
   Chaser(IPathfinder& pathfinder, Position position, Direction direction, const Player& player);
   
   void update(Maze &maze, float deltaTime) override;
   EnemyType getType() const override{ return EnemyType::CHASER ;}
   bool isDefeatable(const Direction&) const override{
      return false;
   };
   
};
