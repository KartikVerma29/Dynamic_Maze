#pragma once

class Maze;

class IMazeGenerator{
public:
   virtual void generate(Maze& maze) = 0;
   virtual ~IMazeGenerator() = default;
};
