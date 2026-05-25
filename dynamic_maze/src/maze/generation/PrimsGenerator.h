#pragma once
#include "IMazeGenerator.h"

class PrimsGenerator: public IMazeGenerator{
public:
   void generate(Maze &maze) override;
};

