/* Your code here! */

#include "maze.h"
using namespace cs225;

SquareMaze::SquareMaze() {
    width = 0;
    height = 0;
    rightWalls = std::vector<std::vector<bool>>(width, std::vector<bool>(width, true));
    downWalls = std::vector<std::vector<bool>>(width, std::vector<bool>(height, true));
}

void SquareMaze::makeMaze(int width, int height) {
    srand(static_cast<unsigned int>(std::time(nullptr)));
    this->width = width;
    this->height = height;
    rightWalls = std::vector<std::vector<bool>>(width, std::vector<bool>(height, true));
    downWalls = std::vector<std::vector<bool>>(height, std::vector<bool>(width, true));
    
    DisjointSets ds;
    ds.addelements(width * height);
    std::chrono::high_resolution_clock::now().time_since_epoch().count();
    while (ds.size(0) < width * height) {
        int x = rand() % width;
        int y = rand() % height;
        int dir = rand() % 2;
        if(dir == 0) {
            if (x < width - 1) {  // ==
                int set1 = y * width + x;
                int set2 = y * width + (x + 1);
                if (ds.find(set1) != ds.find(set2)) {
                    ds.setunion(set1, set2);
                    setWall(x,y,0,false);
                }
            }
        } else {
            if (y < height - 1) { // ||
                int set1 = y * width + x;
                int set2 = (y + 1) * width + x;
                if (ds.find(set1) != ds.find(set2)) {
                    ds.setunion(set1, set2);
                    setWall(x,y,1,false);
                }
            }
        }
        
        
        
    }
    
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if (dir == 0)
        rightWalls[y][x] = exists;
    else if (dir == 1)
        downWalls[y][x] = exists;
}


bool SquareMaze::canTravel(int x, int y, int dir) const {
    if (dir == 0)
        return (x < width - 1) && !rightWalls[y][x];
    else if (dir == 1)
        return (y < height - 1) && !downWalls[y][x];
    else if (dir == 2)
        return (x > 0) && !rightWalls[y][x - 1];
    else if (dir == 3)
        return (y > 0) && !downWalls[y - 1][x];
    return false;
}






cs225::PNG* SquareMaze::drawMaze() {
    int imgWidth = width * 10 + 1;
    int imgHeight = height * 10 + 1;
    cs225::PNG* maze = new cs225::PNG(imgWidth, imgHeight);
    
    for (int x = 10; x < imgWidth; x++)
        maze->getPixel(x, 0) = cs225::HSLAPixel(0, 0, 0);
    
    for (int y = 0; y < imgHeight; y++)
        maze->getPixel(0, y) = cs225::HSLAPixel(0, 0, 0);
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (rightWalls[y][x]) {
                for (int k = 0; k <= 10; k++)
                    maze->getPixel((x + 1) * 10, y * 10 + k) = cs225::HSLAPixel(0, 0, 0);
            }
            
            if (downWalls[y][x]) {
                for (int k = 0; k <= 10; k++)
                    maze->getPixel(x * 10 + k, (y + 1) * 10) = cs225::HSLAPixel(0, 0, 0);
            }
        }
    }
    return maze;
}

cs225::PNG* SquareMaze::drawMazeWithSolution() {
    cs225::PNG* maze = drawMaze(); 
    std::vector<int> solution = solveMaze();

    int x = 5, y = 5;
    for (int dir : solution) {
    for (int i = 0; i < 10; ++i) {
        cs225::HSLAPixel & pixel = maze->getPixel(x, y);
        pixel.h = 0; 
        pixel.s = 1;
        pixel.l = 0.5;
        pixel.a = 1;

        if (dir == 0) x++; // ->
        else if (dir == 1) y++; // !^
        else if (dir == 2) x--; // <-
        else if (dir == 3) y--; // ^
    }
}

if (!solution.empty()) {
    int finalDir = solution.back();
    
    if (x >= 0 && x < static_cast<int>(maze->width()) && y >= 0 && y < static_cast<int>(maze->height())) {
        cs225::HSLAPixel & pixel = maze->getPixel(x, y);
        pixel.h = 0; //🈲
        pixel.s = 1;
        pixel.l = 0.5;
        pixel.a = 1;
    }
    if (finalDir == 0) x++;
    else if (finalDir == 1) y++;
    else if (finalDir == 2) x--;
    else if (finalDir == 3) y--;
    
}

     return maze;
}





std::vector<int> SquareMaze::solveMaze() {
    std::vector<int> dist, prev;
    std::queue<int> q;
    initializeSearch(dist, prev, q);
    performBFS(dist, prev, q);
    int end = findFurthestBottomCell(dist);
    return retracePath(prev, 0, end);
}

void SquareMaze::initializeSearch(std::vector<int>& dist, std::vector<int>& prev, std::queue<int>& q) {
    int n = width * height;
    dist.assign(n, -1);
    prev.assign(n, -1);
    q.push(0);
    dist[0] = 0;
}
int SquareMaze::findFurthestBottomCell(const std::vector<int>& dist) {
    int max = INT_MIN; end = 0;
    for (int i = 0; i < static_cast<int> (width); i++) {
        int u = (height - 1) * width + i;
        if (dist[u] > max) {
            max = dist[u];
            end = u;
        }
    }
    return end;
}



void SquareMaze::performBFS(std::vector<int>& dist, std::vector<int>& prev, std::queue<int>& q) {
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        int x = u % width;
        int y = u / width;
        for (int i = 0; i < 4; i++) {
            int dx = x + xDir[i];
            int dy = y + yDir[i];
            int dis = dy * width + dx;
            if (canTravel(x,y, i)) {
                
                if (dist[dis] == -1) {
                    dist[dis] = dist[u] + 1;
                    prev[dis] = u;
                    q.push(dis);
                }
            }
        }
    }
}

std::vector<int> SquareMaze::retracePath(const std::vector<int>& prev, int start, int end) {
    std::vector<int> path;
    for (auto u = end; u != start; u = prev[u]) {
        int pua = prev[u];
        int dirx = pua % width - u % width, diry = pua / width - u / width;
        for (int i = 0; i < 4; i++) {
            if (dirx == -xDir[i] && diry == -yDir[i]) {
                path.push_back(i);
                break;
            }
        }
    }
    std::reverse(path.begin(), path.end());
    return path;
}




