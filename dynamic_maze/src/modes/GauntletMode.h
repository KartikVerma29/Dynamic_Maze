#pragma once
#include "IGameMode.h"
#include "../enemy/IEnemy.h"
#include "../player/Lives.h"
#include "../collision/CollisionDetector.h"

class GauntletMode:public IGameMode
{
private:
   std::vector<IEnemy*> enemies;
   Lives lives;
   float survivalTime;
   CollisionDetector collisionDetector;
public:
   void init() override;
   void cleanup() override;
   void onEnter() override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer& renderer) override;
   bool isFinished() const override;
};
