#include "BFSSolvabilityChecker.h"
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <unordered_map>
#include <vector>
#include "../Maze.h"
/*
 1  procedure BFS(G, root) is
 2      let Q be a queue
 3      label root as explored
 4      Q.enqueue(root)
 5      while Q is not empty do
 6          v := Q.dequeue()
 7          if v is the goal then
 8              return v
 9          for all edges from v to w in G.adjacentEdges(v) do
10              if w is not labeled as explored then
11                  label w as explored
12                  w.parent := v
13                  Q.enqueue(w)

*/

bool BFSSolvabilityChecker::isSolvable(Maze& maze,const Position& start,const Position& exit){
   return !getPath(maze, start, exit).empty();
}

std::vector<Position> BFSSolvabilityChecker::getPath(Maze& maze,const Position& start,const Position& exit){
   std::queue<Cell*> q;
   std::unordered_map<Cell*, Cell*> parent;

   Cell* startCell = maze.getCell(start.getX(), start.getY());
   Cell* exitCell = maze.getCell(exit.getX(), exit.getY());

   q.push(startCell);
   parent[startCell] = NULL;

   while(!q.empty()){
      Cell* curr = q.front(); q.pop();
      
      if(curr==exitCell) return reconstructPath(parent, startCell, exitCell);

      for(auto dir:{DirectionType::UP, DirectionType::RIGHT, DirectionType::DOWN, DirectionType::LEFT}){
         Wall* wall = curr->getWall(dir);
         if(wall && wall->getIsOpen()){
            Direction d(dir);
            Position offset = d.toVector();
            Cell*neighbour = maze.getCell(curr->getRow()+offset.getX(), curr->getCol()+offset.getY());
         
            if(neighbour && parent.find(neighbour)==parent.end()){
               parent[neighbour] = curr;
               q.push(neighbour);
            }
         }
      }

   }
   return {};
}


std::vector<Position> BFSSolvabilityChecker::reconstructPath(std::unordered_map<Cell*, Cell*>& parent, Cell* start, Cell* exit){
   std::vector<Position> path;
   Cell* curr = exit;
   while(curr!=NULL){
      path.push_back(Position(curr->getRow(), curr->getCol()));
      curr = parent[curr];
   }
   std::reverse(path.begin(), path.end());
   return path;
}
