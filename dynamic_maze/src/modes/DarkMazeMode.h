#pragma once
#include "IGameMode.h"

class DarkMazeMode: public IGameMode
{
private:
   float lightRadius;
   bool isDynamic;
   float mapCoverage;
public:
   void init() override;
   void cleanup() override;
   void onEnter() override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer& renderer) override;
   bool isFinished() const override;
};
