#pragma once
#include "IScoreCalculator.h"

class DynamicMazeScoreCalculator: public IScoreCalculator
{
private:
   int calcTimePenalty(const ScoreContext& ctx){
      return (int)(ctx.timeTaken*10);
   }
   int calcPathScore(const ScoreContext& ctx){
      if(ctx.pathLength==0) return 0;
      return (float)ctx.optimalPathLenght/ctx.pathLength*100;
   }
   int score=0;
public:
   int calculate(const ScoreContext &ctx) override{
      score = calcPathScore(ctx) + calcTimePenalty(ctx);
      return score;
   }
   int getScore() const override{
      return score;
   }
   void resetScore() override{ score=0;}

};
