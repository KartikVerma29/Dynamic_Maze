#pragma once
#include "IAppState.h"
#include "../maze/Maze.h"
#include "../player/Player.h"
#include "../maze/mutation/IMazeMutator.h"
#include "../scoring/IScoreCalculator.h"
#include "../events/EventManager.h"
#include "../maze/generation/IMazeGenerator.h"

class IGameMode: public IAppState{
protected:
   IMazeMutator& mutator;
   IScoreCalculator& scorer;
   EventManager& eventManager;
   std::unique_ptr<Player> player; //init
   std::unique_ptr<Maze> maze;     //init
   IMazeGenerator& generator;

   bool finished=false;
   int stepCounter=0; //Common
   int totalsteps=0;
   float elapsedTime = 0.0f;
   int currentLevel=1;
   int stepThreshold=10;

   int calcMazeSize(int level){
      float maxSize = 50.0f;
      float k=0.5f;
      return (int)(maxSize*(1-std::exp(-k*level)));
   }

public:
   IGameMode(IMazeMutator& mutator, IScoreCalculator& scorer, EventManager& eventManager, IMazeGenerator& generator, UIManager& uiManager):
      mutator(mutator), scorer(scorer), eventManager(eventManager), generator(generator), IAppState(uiManager) {}
 
   virtual void init()=0;
   virtual void cleanup()=0;
   bool isFinished() const override{ return finished;}
   
   AppStateType getNextState() const override { return AppStateType::MAINMENU; }
   AppStateType getType() const override { return AppStateType::MAINMENU; }
   virtual ~IGameMode() = default;
};
