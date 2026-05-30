#pragma once
#include "IEnemy.h"

class Blocker : public IEnemy{
private:
   bool isMoving=false;
public:
   Blocker(Position position, Direction direction);
   void update(Maze &maze, float deltaTime) override;
   bool isDefeatable(const Direction &direction) const override;
};
