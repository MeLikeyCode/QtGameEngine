#ifndef MAPGRID_H
#define MAPGRID_H

#include <vector>
#include <QPointer>
#include "Map.h"

class Node;

/// Represents a grid of maps. You can get/set the Map at each Node ("cell") of
/// the grid. The top left node of the grid is (0,0), bottom right is
/// (numMapsHorizontally -1, numMapsVertically -1).
///
class MapGrid
{
public:
    MapGrid(int numMapsHorizontally, int numMapsVertically);

    // setters
    void setMapAtPos(Map* map, int xPos, int yPos);
    void setMapAtPos(Map* map, const Node& atPos);

    // getters
    Map* mapAt(int xPos, int yPos) const;
    bool contains(Map* map) const;
    Node positionOf(Map* map) const;
    std::vector<Map*> maps() const;

private:
    std::vector<QPointer<Map>> maps_;
    int numMapsHorizontally_;
    int numMapsVertically_;
};

#endif // MAPGRID_H
