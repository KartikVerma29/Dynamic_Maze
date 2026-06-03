#pragma once
#include "IScoreCalculator.h"

class DarkMazeScoreCalculator: public IScoreCalculator
{
private:
   int calcMapCoverage(const ScoreContext& ctx){
      return ctx.mapCoverage*100;
   }
   int calcPathScore(const ScoreContext& ctx){
      if (ctx.optimalPathLenght==0) return 0;
      if(ctx.pathLength==0) return 0;
      return ((float)ctx.optimalPathLenght/ctx.pathLength)*100;
   }
   int score=0;
public:
   int calculate(const ScoreContext &ctx) override{
      score = calcPathScore(ctx) + calcMapCoverage(ctx)*(ctx.pathLength/ctx.optimalPathLenght);
      return score;
   }
   int getScore() const override{
      return score;
   }
}; 


