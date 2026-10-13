#include "PatrolPathfinder.h"
#include "IPathfinder.h"

PatrolPathfinder::PatrolPathfinder(std::vector<Position> route,IPathfinder& finder ,int currentTarget): route(std::move(route)), finder(finder),currentTarget(currentTarget){}

std::vector<Position> PatrolPathfinder::findPath(Maze& maze, Position& from,const Position&) { // 'to' ignored
   if(route.empty()) return {};
   
   if(from.distanceTo(route[currentTarget]) <1.0f ) currentTarget = (currentTarget+1)%(int)route.size();
   return finder.findPath(maze, from, route[currentTarget]);
}

