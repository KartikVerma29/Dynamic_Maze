#pragma once
#include "IMazeGenerator.h"

class RecursiveBacktrackerGenerator : public IMazeGenerator{
private:
   void dfs(Maze& maze, int row, int col);
public:
  void generate(Maze &maze) override;
};
