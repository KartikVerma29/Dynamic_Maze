#include "BFSSolvabilityChecker.h"
#include "../../core/Profiling.h"

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

bool BFSSolvabilityChecker::isSolvable(Maze& maze, const Position& start, const Position& exit) {
   PROFILE_SCOPE;
   int rows = maze.getRows(), cols = maze.getCols();
   Cell* startCell = maze.getCell(start.getX(), start.getY());
   Cell* exitCell  = maze.getCell(exit.getX(), exit.getY());
   if (!startCell || !exitCell) return false;
   std::vector<uint8_t> visited(rows * cols, 0);
   std::queue<Cell*> q;
   q.push(startCell);
   visited[startCell->getRow() * cols + startCell->getCol()] = 1;
   while (!q.empty()) {
      Cell* curr = q.front();
      q.pop();
      if (curr == exitCell) return true;
      for (auto dir : ALL_DIRECTIONS) {
         Wall* wall = curr->getWall(dir);
         if (wall && wall->getIsOpen()) {
            Position offset = Direction(dir).toVector();
            int nr = curr->getRow() + offset.getX();
            int nc = curr->getCol() + offset.getY();
            Cell* neighbour = maze.getCell(nr, nc);
            if (neighbour && !visited[nr * cols + nc]) {
               visited[nr * cols + nc] = 1;
               q.push(neighbour);
            }
         }
      }
   }
   return false;
}

std::vector<Position> BFSSolvabilityChecker::getPath(Maze& maze,const Position& start,const Position& exit){
   PROFILE_SCOPE;
   int rows=maze.getRows(), cols=maze.getCols();
   Cell* startCell = maze.getCell(start.getX(), start.getY());
   Cell* exitCell = maze.getCell(exit.getX(), exit.getY());

   if(!startCell || !exitCell) return {};

   std::vector<Cell*> parent(rows*cols,nullptr);
   std::vector<uint8_t> visited(rows*cols,0);
   std::queue<Cell*> q;

   int startIdx = startCell->getRow()*cols+startCell->getCol();
   q.push(startCell);
   visited[startIdx]=1;

   while(!q.empty()){
      Cell* curr = q.front(); q.pop();
      
      if(curr==exitCell) return reconstructPath(parent, startCell, exitCell, cols);

      for(auto dir:ALL_DIRECTIONS){
         Wall* wall = curr->getWall(dir);
         if(wall && wall->getIsOpen()){
            Position offset = Direction(dir).toVector();
            int nr = curr->getRow()+offset.getX(), nc = curr->getCol()+offset.getY();
            Cell*neighbour = maze.getCell(nr,nc );

               if(neighbour){
                  int nIdx = nr*cols+nc;
                  if(!visited[nIdx]){
                     visited[nIdx]=1;
                     parent[nIdx] = curr;
                     q.push(neighbour);
                  }
            }
         }
      }

   }
   return {};
}

std::vector<Position> BFSSolvabilityChecker::reconstructPath(const std::vector<Cell*>& parent, Cell* start, Cell* exit, int cols){
   std::vector<Position> path;
   Cell* curr = exit;
   while(curr!=nullptr && curr!=start){
      path.push_back(Position(curr->getRow(), curr->getCol()));
      int idx = curr->getRow()*cols + curr->getCol();
      curr = parent[idx];
   }
   if(curr==start) path.push_back(Position(start->getRow(), start->getCol()));
   std::reverse(path.begin(), path.end());
   return path;
}
