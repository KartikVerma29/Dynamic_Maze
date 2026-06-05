#pragma once
#include "IAppState.h"
#include "IGameMode.h"
class DynamicMazeMode:public IGameMode
{
private:
   int currentLevel;
   int stepCounter;
public:
   void init() override;
   void cleanup() override;
   void onEnter() override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer& renderer) override;
   bool isFinished() const override;
};

