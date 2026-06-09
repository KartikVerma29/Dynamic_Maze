#pragma once
#include "IGameMode.h"
#include "../maze/solvability/BFSSolvabilityChecker.h"
#include "../events/IEventListener.h"

class DynamicMazeMode:public IGameMode, public IEventListener<PlayerMovedEvent>
{
private:
   Position exitPos;
   BFSSolvabilityChecker checker;
   std::vector<Position> optimalPath;

public:

   DynamicMazeMode(UIManager uiManager, IMazeMutator& mutator, IScoreCalculator& scorer, EventManager& eventManager, IMazeGenerator& generator, int startLevel=1):
                  IGameMode(mutator, scorer, eventManager, generator, uiManager), exitPos(0,0) {
                     currentLevel=startLevel;
                  }

   void init() override;
   void cleanup() override;
   void onEnter() override;
   void onExit() override;
   void onEvent(const PlayerMovedEvent& event) override;
   void update(float deltaTime) override;
   void render(IRenderer& renderer) override;
};

