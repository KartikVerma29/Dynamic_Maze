#pragma once
#include "IScoreCalculator.h"
#include "../events/events/EnemyDefeatedEvent.h"
#include "../events/IEventListener.h"

class GauntletScoreCalculator:public IScoreCalculator, public IEventListener<EnemyDefeatedEvent>
{
private:
   int calcTimeSurvived(const ScoreContext& ctx){
      return (int)(ctx.timeTaken*20);
   }

   int enemiesDefeated=0;
   int score=0;
public:

   int calculate(const ScoreContext &ctx) override{
      score = calcTimeSurvived(ctx)+enemiesDefeated*100;
      return score;
   }
   void onEvent(const EnemyDefeatedEvent& event) override{
      enemiesDefeated++;
   }
   int getScore() const override{
      return score;
   }
};
