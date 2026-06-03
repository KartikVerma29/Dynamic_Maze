#pragma once

#include <ios>
class Maze;
class Position;

class IMazeMutator{
public:
  virtual void mutate(Maze& maze,const Position& playerPos,const Position& exitPos) = 0;
  virtual ~IMazeMutator() = default;
};
