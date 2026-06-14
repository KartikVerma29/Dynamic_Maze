#pragma once
#include "IGameMode.h"
#include "../maze/solvability/BFSSolvabilityChecker.h"
#include "../events/IEventListener.h"
#include "events/events/PlayerMovedEvent.h"


class DarkMazeMode: public IGameMode, public IEventListener<PlayerMovedEvent>
{
private:
   float lightRadius;
   bool isDynamic;
   float mapCoverage;
   Position exitPos;
   BFSSolvabilityChecker checker;
   std::vector<Position> optimalPath;
public:
  
   DarkMazeMode(UIManager uiManager ,IMazeMutator& mutator, IScoreCalculator& scorer, EventManager& eventManager, IMazeGenerator& generator, int startLevel=1): 
               IGameMode(mutator, scorer, eventManager, generator, uiManager) {
         currentLevel=startLevel;
      }
      
   void init() override;
   void cleanup() override;
   void onEnter() override;
   void onExit() override;
   void update(float deltaTime) override;
   void render(IRenderer& renderer) override;
   void onEvent(const PlayerMovedEvent& event) override;
};
