#pragma once

struct ScoreContext{
   float timeTaken;
   int pathLength;
   int optimalPathLenght;
   int enemiesDefeated;
   float mapCoverage;
};

class IScoreCalculator{
public:
   virtual int calculate(const ScoreContext& ctx ) = 0;
   virtual int getScore()const = 0;
   virtual ~IScoreCalculator() = default;
};
