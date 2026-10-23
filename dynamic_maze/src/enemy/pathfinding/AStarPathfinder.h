#pragma once
#include "IPathfinder.h"
#include <vector>
#include "heuristic/IHeuristic.h"
#include <algorithm>
#include <functional>
#include <queue>
#include <unordered_map>

class AStarPathfinder: public IPathfinder
{
private:
   IHeuristic& heuristic;
public:
   AStarPathfinder(IHeuristic& heuristic): heuristic(heuristic) {}
   std::vector<Position> findPath(Maze &maze, Position &from,const Position &to) override;
   
   ~AStarPathfinder() = default;
};

