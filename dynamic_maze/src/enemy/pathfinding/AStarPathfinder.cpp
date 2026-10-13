#include "AStarPathfinder.h"
#include <algorithm>
#include <functional>
#include <queue>
#include <unordered_map>


struct ANode{
   Cell* cell; 
   float gCost;
   float fCost;

   bool operator>(const ANode& other) const{
      return fCost>other.fCost;
   }

};

std::vector<Position> AStarPathfinder::findPath(Maze& maze, Position& from,const Position& to){
   std::vector<Position> path;
   std::priority_queue<ANode, std::vector<ANode>, std::greater<ANode>> openSet;
   std::unordered_map<Cell*, Cell*> parent;
   std::unordered_map<Cell*, float> gCost;
   
   Cell* start = maze.getCell(from.getX(), from.getY());
   Cell* goal = maze.getCell(to.getX(), to.getY());
   if(!start || !goal) return {};
   if(start==goal) return {from};

   
   gCost[start] = 0.0f;

   Position goalPos(goal->getRow(), goal->getCol());
   openSet.push({start, 0.0f, heuristic.estimate(Position(start->getRow(),start->getCol()), goalPos) });


   while(!openSet.empty()){
      ANode curr = openSet.top(); openSet.pop();
      if(curr.gCost>gCost[curr.cell]) continue;
      if(curr.cell==goal){
         Cell* node = goal;
         while(node!=start){
            path.push_back(Position(node->getRow(), node->getCol()));
            node = parent[node];
         }
         path.push_back(from);
         std::reverse(path.begin(), path.end());
         return path;
      }
   

      for( auto dir:ALL_DIRECTIONS){
         if(!curr.cell->hasOpenWall(dir)) continue;

         auto offset = Direction(dir).toVector();
         int nr = curr.cell->getRow()+offset.getX();
         int nc = curr.cell->getCol()+offset.getY();

         Cell* neighbor = maze.getCell(nr,nc);
         if(!neighbor) continue;

         float newG = gCost[curr.cell]+1.0f;
         if(gCost.count(neighbor) && gCost[neighbor]<=newG) continue;

         gCost[ neighbor] = newG;
         parent[neighbor] = curr.cell;

         float cost = newG + heuristic.estimate(Position(nr,nc), goalPos);
         openSet.push({neighbor, newG, cost});
      }
   }
   return {};
}
