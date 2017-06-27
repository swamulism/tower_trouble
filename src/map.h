#include "objects.h"
#include <vector>
#include <queue>

#ifndef map_H
#define map_H

struct Position {
    int x, y;
    Position(int x, int y): x(x), y(y) {}
};

enum class TileType {
    NOTHING,
    GROUND,
    TOWER1,
    TOWER2,
};

struct Tile {
    TileType tileType;
    Tile(TileType tileType): tileType(tileType) {}
};

// from http://www.redblobgames.com/pathfinding/a-star/implementation.html
template<typename T, typename priority_t>
struct PriorityQueue {
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>,
                        std::greater<PQElement>> elements;

    inline bool empty() const { return elements.empty(); }

    inline void put(T item, priority_t priority) {
        elements.emplace(priority, item);
    }

    inline T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

class Map
{
    int waves[9][10] = {{2, 2, 2, 2, 2, 0, 0, 0, 0, 0},
                              {2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                              {1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
                              {2, 1, 2, 1, 2, 1, 2, 1, 2, 1},
                              {2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
                              {3, 0, 3, 0, 3, 0, 3, 0, 3, 0},
                              {3, 2, 1, 3, 2, 1, 3, 2, 1, 3},
                              {3, 3, 3, 3, 2, 2, 2, 1, 1, 1},
                              {3, 3, 3, 3, 3, 3, 3, 3, 3, 3}};
    int wavetime = 600;
    int currentwavetime = 0;
    int spawncount = -1;
    int lives;

    int width, height;
    std::vector<std::vector<Tile>> map;

    void makeMap();
public:
    int currentwave = -1;
    void renderTile(float x, float y, float z);
    void render();
    int spawnEnemy();
    void spawnWave();
    int getlocation(float ex, float ey);

    Map(int width, int height, int lives);
    int getWidth() const;
    int getHeight() const;
    int getLives() const;
    void setLives(int x);
    void decrementLives();
    Tile & operator[](Position pos);
    const Tile & operator[](Position pos) const;
    Tile getTile(Position pos) const;
    void setTile(Tile tile, Position pos);
    bool isEmpty(Position pos);
    bool setTower(TileType tower, Position pos);
    std::vector<Position> getNeighbors(Position pos);
    float getHValue(Position cur, Position end);
    std::vector<Position> AStar(Position start, Position end);
};

#endif
