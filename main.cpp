#include <iostream>
#include "dsets.h"
#include "maze.h"
#include "PNG.h"

using namespace std;
using namespace cs225;
int main()
{
    int size = ...   // enter the size of your expected matrix
    SquareMaze maze;
    maze.makeMaze(size, size);
    PNG * actualmaze = maze.drawMaze();
    actualmaze->writeToFile("..."); //enter the file address
    maze.solveMaze();
    PNG * actualOutput = maze.drawMazeWithSolution();
    actualOutput->writeToFile("...");//enter the file address

    delete actualOutput;
    delete actualmaze;
    return 0;
}
