#include "AStarPathfinder.h"
#include "../../core/Profiling.h"
#include <limits>

struct ANode{
   Cell* cell; 
   float gCost;
   float fCost;

   bool operator>(const ANode& other) const{
      return fCost>other.fCost;
   }

};

std::vector<Position> AStarPathfinder::findPath(Maze& maze, Position& from,const Position& to){
   PROFILE_SCOPE;
   int rows = maze.getRows(), cols = maze.getCols();
   Cell* start = maze.getCell(from.getX(), from.getY());
   Cell* goal = maze.getCell(to.getX(), to.getY());
   if(!start || !goal) return {};
   if(start==goal) return {from};

   std::vector<Position> path;
   std::priority_queue<ANode, std::vector<ANode>, std::greater<ANode>> openSet;
   std::vector<Cell*> parent(rows*cols, nullptr);
   std::vector<float> gCost(rows*cols, std::numeric_limits<float>::infinity());
   
   int startIdx = start->getRow()*cols+start->getCol();
   gCost[startIdx] = 0.0f;

   Position goalPos(goal->getRow(), goal->getCol());
   openSet.push({start, 0.0f, heuristic.estimate(Position(start->getRow(),start->getCol()), goalPos) });


   while(!openSet.empty()){
      ANode curr = openSet.top(); openSet.pop();

      int currIdx = curr.cell->getRow()*cols + curr.cell->getCol();
      if(curr.gCost>gCost[currIdx]) continue;
      if(curr.cell==goal){
         Cell* node = goal;
         while(node!=start){
            path.push_back(Position(node->getRow(), node->getCol()));
            node = parent[node->getRow()*cols+node->getCol()];
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

         int nIdx = nr*cols+nc;
         float newG = gCost[currIdx]+1.0f;
         if(newG>=gCost[nIdx]) continue;

         gCost[nIdx] = newG;
         parent[nIdx] = curr.cell;

         float cost = newG + heuristic.estimate(Position(nr,nc), goalPos);
         openSet.push({neighbor, newG, cost});
      }
   }
   return {};
}
