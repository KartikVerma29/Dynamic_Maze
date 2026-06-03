#pragma once
#include "IScoreCalculator.h"

class GauntletScoreCalculator:public IScoreCalculator
{
private:
   int calcTimeSurvived(const ScoreContext& ctx){
      return (int)(ctx.timeTaken*20);
   }

   int calcEnemiesDefeated(const ScoreContext& ctx){
      return ctx.enemiesDefeated*100;
   }

   int score=0;
public:

   int calculate(const ScoreContext &ctx) override{
      score = calcEnemiesDefeated(ctx) + calcTimeSurvived(ctx);
      return score;
   }

   int getScore() const override{
      return score;
   }
};
