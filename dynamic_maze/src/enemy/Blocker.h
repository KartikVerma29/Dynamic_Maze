#pragma once
#include "IEnemy.h"

class Blocker : public IEnemy{
private:
   bool isMoving=false;
   float idleTimer;
   int stepsRemaining;
   Position targetPos;
   
   Position findCorridorTarget(Maze& maze) const;

public:
   
   Blocker(Position position, Direction direction) : IEnemy(position,direction), idleTimer(3.0f), stepsRemaining(0){}

   void update(Maze &maze, float deltaTime) override;
   bool isDefeatable(const Direction &direction) const override {return isMoving;}
};
