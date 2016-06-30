#ifndef MAPGRID_H
#define MAPGRID_H

#include <vector>

class Map;
class Node;

class MapGrid
{
public:
    MapGrid(int numMapsHorizontally, int numMapsVertically);

    void insertMap(Map* map, int xPos, int yPos);
    Map* mapAt(int xPos, int yPos);
    bool contains(Map* map);
    Node positionOf(Map* map);
    std::vector<Map*> maps();

private:
    std::vector<Map*> maps_;
    int numMapsHorizontally_;
    int numMapsVertically_;
};

#endif // MAPGRID_H
