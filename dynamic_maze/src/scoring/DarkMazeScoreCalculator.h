#pragma once
#include "IScoreCalculator.h"

class DarkMazeScoreCalculator: public IScoreCalculator
{
private:
   int calcMapCoverage(const ScoreContext& ctx){
      return (int)(ctx.mapCoverage*3000.0f);
   }

   int calcTimePenalty(const ScoreContext& ctx){
      if(ctx.optimalPathLenght==0) return 0;
      float secondsPerOptimalStep = ctx.timeTaken / (float)ctx.optimalPathLenght;
      return (int)(secondsPerOptimalStep * 150.0f);
   }
   int score=0;
public:
   int calculate(const ScoreContext &ctx) override{
      int coverage = calcMapCoverage(ctx);
      int tPenalty = calcTimePenalty(ctx);
      int base = 500;
      score = std::max(0, base+coverage-tPenalty);
      return score;
   }
   int getScore() const override{
      return score;
   }
   void resetScore() override{
      score=0;
   }
}; 


