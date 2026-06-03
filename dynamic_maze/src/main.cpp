#include <iostream>
#include "maze/Maze.h"
#include "maze/generation/RecursiveBacktrackerGenerator.h"
#include "maze/generation/PrimsGenerator.h"
#include "core/Direction.h"
#include "maze/mutation/GridMazeMutator.h"
#include "maze/solvability/BFSSolvabilityChecker.h"

using namespace std;

void printMaze(Maze& maze) {
    int rows = maze.getRows(), cols = maze.getCols();

    // print top border
    for (int c = 0; c < cols; c++) std::cout << "+---";
    std::cout << "+\n";

    for (int r = 0; r < rows; r++) {
        // print left border + horizontal walls
        std::cout << "|";
        for (int c = 0; c < cols; c++) {
            Cell* cell = maze.getCell(r, c);
            std::cout << "   ";
            // right wall
            Wall* right = cell->getWall(DirectionType::RIGHT);
            if (right && right->getIsOpen())
                std::cout << " ";
            else
                std::cout << "|";
        }
        std::cout << "\n";

        // print bottom walls
        for (int c = 0; c < cols; c++) {
            Cell* cell = maze.getCell(r, c);
            Wall* down = cell->getWall(DirectionType::DOWN);
            if (down && down->getIsOpen())
                std::cout << "+   ";
            else
                std::cout << "+---";
        }
        std::cout << "+\n";
    }
}

int main() {
    float row,col;
    cin>>row>>col;
    Maze maze(row, col);
    RecursiveBacktrackerGenerator gen;
    gen.generate(maze);
    BFSSolvabilityChecker checker;
    EventManager eventManager;

    Position playerPos{0, 0};
    Position exitPos{row-1, col-1};

    GridMazeMutator mutator(checker, eventManager);

    std::cout << "=== RecursiveBacktracker ===\n";
    printMaze(maze);


    bool solvable = checker.isSolvable(maze, playerPos, exitPos);
    std::cout << "Is maze solvable? " << (solvable ? "Yes" : "No") << "\n";


    auto path = checker.getPath(maze, playerPos, exitPos);
    std::cout << "Path Length: " << path.size() << "\n";
    

    mutator.mutate(maze, playerPos, exitPos);
        std::cout << "\n=== After Mutation ===\n";
    printMaze(maze);
    std::cout << "Is maze solvable after mutation? " << (checker.isSolvable(maze, playerPos, exitPos) ? "Yes" : "No") << "\n";


    // Maze maze2(10, 10);
    // PrimsGenerator prim;
    // prim.generate(maze2);

    // std::cout << "\n=== Prims ===\n";
    // printMaze(maze2);

    return 0;
}
