#pragma once

#include <vector>
#include <QPointer>

#include "Map.h"
#include "PointerGrid.h"

class Map;

/// Represents a grid of maps. You can get/set the Map at each Node ("cell") of
/// the grid. The top left node of the grid is (0,0), bottom right is
/// (numMapsHorizontally -1, numMapsVertically -1).
/// @author Abdullah Aghazadah
class MapGrid
{
public:
    MapGrid(int numMapsHorizontally = 0, int numMapsVertically = 0);

    // setters
    void setMapAtPos(Map* map, int xPos, int yPos);
    void setMapAtPos(Map* map, const Node& atPos);

    // getters
    Map* mapAt(int xPos, int yPos) const;
    bool contains(Map* map) const;
    Node positionOf(Map* map) const;
    std::vector<Map*> maps() const;

private:
    PointerGrid<Map> pointerGrid_;
};
