#include "RecursiveBacktrackerGenerator.h"
#include <cstdlib>
#include <stack>
#include "../Maze.h"
/*
1. Choose the initial cell, mark it as visited and push it to the stack
2. While the stack is not empty
   1. Pop a cell from the stack and make it a current cell
   2. If the current cell has any neighbours which have not been visited
         1. Push the current cell to the stack
         2. Choose one of the unvisited neighbours
         3. Remove the wall between the current cell and the chosen cell
         4. Mark the chosen cell as visited and push it to the stack

*/

std::vector<std::pair<int,int>> RecursiveBacktrackerGenerator::getUnvisit(Maze& maze, int row, int col) {
    std::vector<std::pair<int,int>> result;
    int rows = maze.getRows(), cols = maze.getCols();

    // UP, DOWN, LEFT, RIGHT offsets
    std::vector<std::pair<int,int>> offsets = {{-1,0},{1,0},{0,-1},{0,1}};
    for (auto [dr, dc] : offsets) {
        int nr = row + dr, nc = col + dc;
        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
            if (!maze.getCell(nr, nc)->isVisited()) {
                result.push_back({nr, nc});
            }
        }
    }
    return result;
}

Wall* RecursiveBacktrackerGenerator::getWallbetween(Cell* cell, int row, int col, int nRow, int nCol) {
    DirectionType dir;
    if (nRow < row) dir = DirectionType::UP;
    else if (nRow > row) dir = DirectionType::DOWN;
    else if (nCol < col) dir = DirectionType::LEFT;
    else dir = DirectionType::RIGHT;
    return cell->getWall(dir);
}

void RecursiveBacktrackerGenerator::generate(Maze& maze){
   int rows = maze.getRows(), cols = maze.getCols();
   int sRow = std::rand()%rows, sCol = std::rand()%cols;
   maze.getCell(sRow, sCol)->setVisited(true);

   std::stack<std::pair<int,int>> st;
   st.push({sRow, sCol});

   while(st.size()){
      auto [row,col] = st.top();

      std::vector<std::pair<int,int>> neighbours = getUnvisit(maze,row,col);

      if(!neighbours.empty()){
         int idx = std::rand()%neighbours.size();
         auto [r,c] = neighbours[idx];

         Cell* curr = maze.getCell(row, col);
         Wall* wall = getWallbetween(curr, row, col, r, c);
         if(wall) wall->open();

         maze.getCell(r,c)->setVisited(true);
         st.push({r,c});
      }else{
         st.pop();
      }

   }

}

