#pragma once
#include "IEnemy.h"

class Blocker : public IEnemy{
private:
   bool isMoving=false;
   float idleTimer;
   float moveTimer=0.0f;
   float moveInterval=0.f;
   int stepsRemaining;
   Position targetPos;
   
   Position findCorridorTarget(Maze& maze) const;

public:
   
   Blocker(Position position, Direction direction) : IEnemy(position,direction,2.5f), idleTimer(3.0f), stepsRemaining(0){}
   EnemyType getType() const override{ return EnemyType::BLOCKER ;}
   void update(Maze &maze, float deltaTime) override;
   void onEvent(const WallStateChangedEvent& event) override;
   bool isDefeatable(const Direction &direction) const override {return isMoving;}
};
