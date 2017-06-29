#include "map.h"
#include <vector>
#include <cmath>
#include <queue>
#include <tuple>
#include <unordered_map>

Map::Map(int width, int height, int lives): width(width), height(height), lives(lives) {
    makeMap();
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

int Map::getLives() const {
    return lives;
}

void Map::setLives(int x) {
    lives = x;
}

void Map::decrementLives() {
    lives = getLives() - 1;
}

Tile & Map::operator[](Position pos) {
    return map[pos.x][pos.y];
}

//const Tile & Map::operator[](Position pos) const {
//    return map[pos.x][pos.y];
//}

Tile Map::getTile(Position pos) const {
    return map[pos.x][pos.y];
}

void Map::setTile(Tile tile, Position pos) {
    map[pos.x][pos.y] = tile;
}

void Map::renderTile(float x, float y, float z) {
    glPushMatrix();

    glTranslated(x, y, z);

    glBegin(GL_TRIANGLES);
    glNormal3d(0.10,1,0);
    glVertex3d(0,1.10,0);
    glVertex3d(1,1,1);
    glVertex3d(1,1,-1);

    glNormal3d(0,1,-0.10);
    glVertex3d(0,1.10,0);
    glVertex3d(1,1,-1);
    glVertex3d(-1,1,-1);

    glNormal3d(-0.10,1,0);
    glVertex3d(0,1.10,0);
    glVertex3d(-1,1,-1);
    glVertex3d(-1,1,1);

    glNormal3d(0,1,0.10);
    glVertex3d(0,1.10,0);
    glVertex3d(-1,1,1);
    glVertex3d(1,1,1);
    glEnd();

    glPopMatrix();
}

void Map::render() {
    glPushMatrix();
    glTranslated(-8,0,-8);

    for (int i = 0; i < getWidth(); ++i) {
        for (int j = 0; j < getHeight(); ++j) {
            renderTile(i * 2, 0, j * 2);
        }
    }
    glPopMatrix();

    if (getLives() > 0) {
        glColor3f(0.4,0.8,0.4);
        float emission[4] = {0.0,0.4,0.0,1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        for (int i = 0; i < lives; ++i) {
            if (i < 2) {
                cube(8.5, 1.5*i, -6.0, 0, 1.0);
            }
            else {
                cube(10.0, 1.5*(i%2), -6.0 + 1.5*((i-2)/2), 0, 1.0);
            }
        }
    }
    else {
        bool pixels[7][53] = {{0,1,1,1,0, 0, 0,1,1,1,0, 0, 0,1,0,1,0, 0, 1,1,1,1,1, 0, 0,0,0,0,0, 0, 0,1,1,1,0, 0, 1,0,0,0,1, 0, 1,1,1,1,1, 0, 1,1,1,1,0},
                                     {1,0,0,0,1, 0, 1,0,0,0,1, 0, 1,0,1,0,1, 0, 1,0,0,0,0, 0, 0,0,0,0,0, 0, 1,0,0,0,1, 0, 1,0,0,0,1, 0, 1,0,0,0,0, 0, 1,0,0,0,1},
                                     {1,0,0,0,0, 0, 1,0,0,0,1, 0, 1,0,1,0,1, 0, 1,0,0,0,0, 0, 0,0,0,0,0, 0, 1,0,0,0,1, 0, 1,0,0,0,1, 0, 1,0,0,0,0, 0, 1,0,0,0,1},
                                     {1,0,1,1,1, 0, 1,1,1,1,1, 0, 1,0,1,0,1, 0, 1,1,1,1,0, 0, 0,0,0,0,0, 0, 1,0,0,0,1, 0, 1,0,0,0,1, 0, 1,1,1,1,0, 0, 1,1,1,1,0},
                                     {1,0,0,0,1, 0, 1,0,0,0,1, 0, 1,0,1,0,1, 0, 1,0,0,0,0, 0, 0,0,0,0,0, 0, 1,0,0,0,1, 0, 1,0,0,0,1, 0, 1,0,0,0,0, 0, 1,0,1,0,0},
                                     {1,0,0,0,1, 0, 1,0,0,0,1, 0, 1,0,1,0,1, 0, 1,0,0,0,0, 0, 0,0,0,0,0, 0, 1,0,0,0,1, 0, 0,1,0,1,0, 0, 1,0,0,0,0, 0, 1,0,0,1,0},
                                     {1,1,1,1,1, 0, 1,0,0,0,1, 0, 1,0,1,0,1, 0, 1,1,1,1,1, 0, 0,0,0,0,0, 0, 0,1,1,1,0, 0, 0,0,1,0,0, 0, 1,1,1,1,1, 0, 1,0,0,0,1}};
        glColor3f(0.9,0.9,0.9);
        glPushMatrix();
        for (float th=0.0; th <= 271.0; th += 90) {
            glRotated(th, 0,1,0);
            for (int i=0; i<53; ++i) {
                for (int j=0; j<7;++j) {
                    if (pixels[j][i])
                        cube(2.5-0.1*i,4-0.1*j,9.0, 0, 0.1/sqrt(2.0));
                }
            }
        }
        glPopMatrix();
    }
}

int Map::spawnEnemy() {
    if (spawncount > 0 && currentwave >= 0) {
        currentwavetime += 16;
        if (currentwavetime >= wavetime) {
            currentwavetime -= wavetime;
            spawncount -= 1;
            return waves[currentwave][9-spawncount];
        }
    }
    return 0;
}

void Map::spawnWave()
{
    if (spawncount <= 0)
    {
        if (currentwave < 8)
            currentwave += 1;
        spawncount = 10;
    }
}

bool Map::isEmpty(Position pos) {
    if (pos.x >= width || pos.x < 0 || pos.y >= height || pos.y < 0) {
        return false;
    }
    return map[pos.x][pos.y].tileType == TileType::GROUND;
}

bool Map::setTower(TileType tower, Position pos) {
    if (isEmpty(pos)) {
        map[pos.x][pos.y].tileType = tower;
        return true;
    }
    return false;
}

std::vector<Position> Map::getNeighbors(Position pos) {
    std::vector<Position> neighbors;
    Position up = Position(pos.x - 1, pos.y);
    Position down = Position(pos.x + 1, pos.y);
    Position left = Position(pos.x, pos.y - 1);
    Position right = Position(pos.x, pos.y + 1);
    if (isEmpty(up)) {
        neighbors.push_back(up);
    }
    if (isEmpty(down)) {
        neighbors.push_back(down);
    }
    if (isEmpty(left)) {
        neighbors.push_back(left);
    }
    if (isEmpty(right)) {
        neighbors.push_back(right);
    }
    return neighbors;
}

float Map::getHValue(Position cur, Position end) {
    return sqrt(pow((cur.x - end.x), 2) + pow((cur.y - end.y), 2));
}


std::vector<Position> Map::AStar(Position start, Position end) {
    std::vector<Position> closed;
    int cost = 0;
    PriorityQueue<Position, float> open;
    open.put(start, getHValue(start, end));
    std::unordered_map<Position, Position> came_from;
    std::unordered_map<Position, float> cost_so_far;
    came_from[start] = start;
    cost_so_far[start] = 0;
    std::vector<Position> path;

    while (!open.empty()) {
        auto cur = open.get();
        if (cur == end) {
            path.push_back(cur);
            while (cur != start) {
                cur = came_from[cur];
                path.push_back(cur);
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }
        for (auto& next : getNeighbors(cur)) {
              float new_cost = cost_so_far[cur] + 1; 
              if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                float priority = new_cost + getHValue(next, end);
                open.put(next, priority);
                came_from[next] = cur;
              }
        }
    }
    return path;
}

void Map::makeMap() {
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            map[i][j].tileType = TileType::GROUND;
            // if(i == 0 || i == width - 1 || j == 0 || j == height - 1) {
            //     map[i][j].tileType = TileType::NOTHING;
            // }
            // else {
            //     map[i][j].tileType = TileType::GROUND;
            // }
        }
    }
}
