#pragma once
#include "IMazeGenerator.h"
#include <cstdlib>
#include "../Maze.h"
#include <vector>

class Wall;

class PrimsGenerator: public IMazeGenerator{
private:
   void addFWalls(Maze& maze, int row, int col, std::vector<Wall*>& fWalls);

public:
   void generate(Maze& maze) override;
};


