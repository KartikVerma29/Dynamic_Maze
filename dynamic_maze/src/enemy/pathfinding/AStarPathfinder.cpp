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

static float heuristic(Cell* a, Cell* b){
   Position posA(a->getRow(), a->getCol());
   Position posB(b->getRow(), b->getCol());
   return posA.distanceTo(posB);
}

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
   openSet.push({start, 0.0f, heuristic(start, goal) });

   auto delta = [](DirectionType d) ->std::pair<int,int>{
      switch (d) {
         case DirectionType::UP  : return {-1,0};
         case DirectionType::DOWN: return {1,0};
         case DirectionType::LEFT: return {0,-1};
         case DirectionType::RIGHT: return {0,1};
      }
      return {0,0};
   };


   while(!openSet.empty()){
      ANode curr = openSet.top(); openSet.pop();

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
   

      for( auto dir:{DirectionType::RIGHT, DirectionType::LEFT, DirectionType::DOWN, DirectionType::UP}){
         if(!curr.cell->hasOpenWall(dir)) continue;

         auto [dr,dc] = delta(dir);
         int nr = curr.cell->getRow()+dr;
         int nc = curr.cell->getCol()+dc;

         Cell* neighbor = maze.getCell(nr,nc);
         if(!neighbor) continue;

         float newG = gCost[curr.cell]+1.0f;
         if(gCost.count(neighbor) && gCost[neighbor]<=newG) continue;

         gCost[ neighbor] = newG;
         parent[neighbor] = curr.cell;

         float cost = newG + heuristic(neighbor, goal);
         openSet.push({neighbor, newG, cost});
      }
   }
   return {};
}
