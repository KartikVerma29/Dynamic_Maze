#pragma once
#include "IGameMode.h"
#include "../maze/solvability/BFSSolvabilityChecker.h"

class DynamicMazeMode:public IGameMode
{
private:
   Position exitPos;
   BFSSolvabilityChecker checker;
   std::vector<Position> optimalPath;

public:

   DynamicMazeMode(IMazeMutator& mutator, IScoreCalculator& scorer, EventManager& eventManager, IMazeGenerator& generator, int startLevel=1):
                  IGameMode(mutator, scorer, eventManager, generator), exitPos(0,0) {
                     currentLevel=startLevel;
                  }

   void init() override;
   void cleanup() override;
   void onEnter() override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer& renderer) override;
};

