#pragma once
#include "IAppState.h"
#include "../maze/Maze.h"
#include "../player/Player.h"
#include "../maze/mutation/IMazeMutator.h"
#include "../scoring/IScoreCalculator.h"
#include "../events/EventManager.h"
#include "../maze/generation/IMazeGenerator.h"

enum class GrowthMode{
   LINEAR, EXPON, SATRU
};

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
   int levelCompleted;
   int calcMazeSize(int level, GrowthMode mode=GrowthMode::LINEAR){
      int maxSize = 55.0f;
      int basSize = 11.0f;
      int newSize = basSize;

      switch(mode){
         case GrowthMode::LINEAR:{
            int growthRate = 4;
            newSize = basSize + ((currentLevel-1)*growthRate);
            break;
         }
         case GrowthMode::EXPON:{
            float multiplier = 1.3f;
            newSize = basSize*std::pow(multiplier,currentLevel-1);
            break;
         }
         case GrowthMode::SATRU:{
            float k=0.5f;
            newSize = (maxSize*(1-std::exp(-k*level)));
            break;
         }
      }

      newSize = std::min(newSize, maxSize);
      if(newSize%2 == 0){
         newSize+=1;
      }
      return newSize;
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
