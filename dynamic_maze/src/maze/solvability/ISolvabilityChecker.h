#pragma once

#include <vector>
class Maze;
class Position;

class ISolvabilityChecker{
public:
   virtual bool isSolvable(Maze& maze, const Position& start, const Position& exit)=0;
   virtual std::vector<Position> getPath(Maze& maze,const Position& start,const Position& exit)=0;
   virtual ~ISolvabilityChecker() = default;
};


