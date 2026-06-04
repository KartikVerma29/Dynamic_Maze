#pragma once
#include "IAppState.h"
#include "../maze/Maze.h"
#include "../player/Player.h"
#include "../maze/mutation/IMazeMutator.h"
#include "../scoring/IScoreCalculator.h"
#include "../events/EventManager.h"

class IGameMode: public IAppState{
protected:
   Maze maze;
   Player player;
   IMazeMutator& mutator;
   IScoreCalculator& scorer;
   EventManager& eventManager;
public:
   IGameMode(Maze maze, Player player, IMazeMutator& mutator, IScoreCalculator& scorer, EventManager& eventManager);

   virtual void init()=0;
   virtual void cleanup()=0;
   virtual ~IGameMode() = default;
};
