#pragma once
#include "IMazePostProcessor.h"
#include <random>
#include "../../core/Cell.h"
#include "../../core/Wall.h"

class MazeBraider: public IMazePostProcessor{
private:
    float braidFactor;
    std::mt19937 rng;
    int minWalls;
    int openCount(Cell* cell) const;
    bool isDeadEnd(Cell* cell) const;
    bool createsOpenBlock(Maze& maze, Cell* cell, DirectionType dir) const;
    Wall* pickClosedNeighbour(Maze& maze, Cell*);
public:
    MazeBraider(float factor, unsigned seed = std::random_device{}(), int minWalls=1): 
    braidFactor(factor), rng(seed), minWalls(minWalls){}
    void process(Maze& maze) override;
};
