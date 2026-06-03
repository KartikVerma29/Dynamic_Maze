#include "PatrolPathfinder.h"

PatrolPathfinder::PatrolPathfinder(std::vector<Position> route, int currentTarget): route(std::move(route)), currentTarget(currentTarget){}

std::vector<Position> PatrolPathfinder::findPath(Maze& maze, Position& from, Position& to) { // 'to' ignored
   if(route.empty()) return {};
   
   if(from.distanceTo(route[currentTarget]) <1.0f ) currentTarget = (currentTarget+1)%(int)route.size();
   return astar.findPath(maze, from, route[currentTarget]);
}

