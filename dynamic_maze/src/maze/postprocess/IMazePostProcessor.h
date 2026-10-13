#pragma once
#include "../Maze.h"

class IMazePostProcessor{
public:
    virtual void process(Maze& maze) = 0;
    virtual ~IMazePostProcessor() = default;
};