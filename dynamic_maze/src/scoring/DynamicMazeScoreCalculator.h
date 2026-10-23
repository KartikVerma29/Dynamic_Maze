#pragma once
#include "IScoreCalculator.h"

class DynamicMazeScoreCalculator: public IScoreCalculator{
private:
   int score=0;
   int calcTimePenalty(const ScoreContext& ctx){
      if(ctx.optimalPathLenght==0) return 0;
      float secPerStep = ctx.timeTaken/(float)ctx.optimalPathLenght;
      return (int)(secPerStep*300.0f);
   }
   int calcPathScore(const ScoreContext& ctx){
      if(ctx.pathLength==0) return 0;
      return (int)(((float)ctx.optimalPathLenght/ctx.optimalPathLenght)*1000.0f);
   }
public:
   int calculate(const ScoreContext &ctx) override{
      int pScore = calcPathScore(ctx);
      int tPenalty = calcTimePenalty(ctx);
      score = std::max(0, pScore-tPenalty);
      return score;
   }
   int getScore() const override{
      return score;
   }
   void resetScore() override{ score=0;}

};
