#pragma once
#include "IMazeGenerator.h"
#include <cstdlib>
#include "../Maze.h"
#include <random>
#include <vector>
#include <cstdlib>

class Wall;

class PrimsGenerator: public IMazeGenerator{
private:
   std::mt19937 rng{std::random_device{}()};
   void addFWalls(Maze& maze, int row, int col, std::vector<Wall*>& fWalls);
public:
   void generate(Maze& maze) override;
};


