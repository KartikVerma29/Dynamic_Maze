#pragma once
#include <vector>
#include "../../events/events/WallStateChangedEvent.h"

class Maze;
class Position;

class IMazeMutator{
  public:
  virtual void mutate(Maze& maze,const Position& playerPos,const Position& exitPos) = 0;
  virtual ~IMazeMutator() = default;
  virtual void update(float deltaTime, Maze& maze, const Position& playerPos, const Position& exitPos)=0;
  virtual void clearPending() = 0;
  // virtual std::vector<WallStateChangedEvent> plan(Maze& maze, Position& playerPos, Position& exitPos) = 0;
  // virtual void commit(std::vector<WallStateChangedEvent> wallChange) = 0;
};
