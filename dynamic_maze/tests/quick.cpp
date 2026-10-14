#include <gtest/gtest.h>
#include "../src/maze/Maze.h"
#include "../src/maze/generation/RecursiveBacktrackerGenerator.h"
#include "../src/maze/solvability/BFSSolvabilityChecker.h"

TEST(Maze, GeneratedMazeIsSolvable){
   Maze m(21,21);
   RecursiveBacktrackerGenerator().generate(m);
   BFSSolvabilityChecker bfs;
   EXPECT_TRUE(bfs.isSolvable(m, Position(0,0), Position(20,20)));
}