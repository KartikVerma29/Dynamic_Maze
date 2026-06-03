#include "PrimsGenerator.h"

/*
1. Start with a grid full of walls.
2. Pick a cell, mark it as part of the maze. Add the walls of the cell to the wall list.
3. While there are walls in the list:
      1. Pick a random wall from the list.
      2. If only one of the cells that the wall divides is visited, then:
            1. Make the wall a passage and mark the unvisited cell as part of the maze.
            2. Add the neighboring walls of the cell to the wall list.
      3. Remove the wall from the list.
 */

void PrimsGenerator::addFWalls(Maze& maze, int row, int col, std::vector<Wall*>& fWalls){
   Cell* cell = maze.getCell(row,col);
   for(const auto dir: {DirectionType::UP, DirectionType::DOWN, DirectionType::LEFT, DirectionType::RIGHT}){
      Wall* wall = cell->getWall(dir);
      if(wall && !wall->getIsOpen()){
         fWalls.push_back(wall);
      }
   }
}


void PrimsGenerator::generate(Maze& maze){
   int rows = maze.getRows(), cols = maze.getCols();
   int sRow = std::rand()%rows, sCol = std::rand()%cols;

   maze.getCell(sRow, sCol)->setVisited(true);

   std::vector<Wall*> fWalls;
   addFWalls(maze, sRow, sCol,fWalls);
   while(! fWalls.empty()){
      int idx = std::rand()%fWalls.size();
      Wall* wall = fWalls[idx];

      auto [cellA, cellB] = wall->getCells();
      Cell*a = maze.getCell(cellA.first, cellA.second);
      Cell*b = maze.getCell(cellB.first, cellB.second);

      if(a->isVisited()!=b->isVisited()){
         wall->open();
         Cell* unvisited = a->isVisited()?b:a;
         auto [r,c] = a->isVisited()?cellB:cellA;
         unvisited->setVisited(true);
         addFWalls(maze, r, c, fWalls);
      }
   
      fWalls.erase(fWalls.begin()+idx);
   }

}
