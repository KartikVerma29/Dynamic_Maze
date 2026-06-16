#pragma once
#include "IScoreCalculator.h"

class DarkMazeScoreCalculator: public IScoreCalculator
{
private:
   int calcMapCoverage(const ScoreContext& ctx){
      return ctx.mapCoverage*1000;
   }
   int calcPathScore(const ScoreContext& ctx){
      if (ctx.optimalPathLenght==0) return 0;
      if(ctx.pathLength==0) return 0;
      return ((float)ctx.optimalPathLenght/ctx.pathLength)*100;
   }
   int calcTimePenalty(const ScoreContext& ctx){
      return 1000/ctx.timeTaken;
   }
   int score=0;
public:
   int calculate(const ScoreContext &ctx) override{
      score = calcPathScore(ctx) + calcMapCoverage(ctx) + calcTimePenalty(ctx);
      return score;
   }
   int getScore() const override{
      return score;
   }
   void resetScore() override{
      score=0;
   }
}; 


