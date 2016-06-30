#include "MapGrid.h"
#include <cassert>
#include <Node.h>

MapGrid::MapGrid(int numMapsHorizontally, int numMapsVertically):
    numMapsHorizontally_(numMapsHorizontally),
    numMapsVertically_(numMapsVertically)
{
    // initialize all maps to nullptr
    for (int i = 0, n = numMapsHorizontally_* numMapsVertically_; i < n; i++){
        maps_.push_back(nullptr);
    }
}

/// Inserts the specified Map in the specified position in the MapGrid.
/// map can be null.
void MapGrid::insertMap(Map *map, int xPos, int yPos)
{
    // make sure xPos and yPos are within bounds
    assert(xPos < numMapsHorizontally_ && yPos < numMapsVertically_);

    int posInArray = yPos * numMapsHorizontally_ + xPos;
    maps_[posInArray] = map;
}

/// Returns the Map at the specified position in the MapGrid.
Map *MapGrid::mapAt(int xPos, int yPos)
{
    // make sure xPos and yPos are within bounds
    assert(xPos < numMapsHorizontally_ && yPos < numMapsVertically_);

    int posInArray = yPos * numMapsHorizontally_ + xPos;
    return maps_[posInArray];
}

/// Returns true if the MapGrid contains the specified map.
bool MapGrid::contains(Map *map)
{
    for (Map* eachMap:maps_){
        if (eachMap == map ){
            return true;
        }
    }
    return false;
}

/// Returns the position of the specified map in the MapGrid.
/// If the specified map is not in the map grid, will throw.
/// Use contains(Map*) to check if the map is in the MapGrid before calling
/// this function.
Node MapGrid::positionOf(Map *map)
{
    assert(contains(map));

    int indexOfMap;
    for (int i = 0, n = maps_.size(); i < n; i++){
        if (maps_[i] == map){
            indexOfMap = i;
            break;
        }
    }

    int yPos = indexOfMap / numMapsHorizontally_;
    int xPos = indexOfMap % numMapsHorizontally_;
    return Node(xPos,yPos);
}

/// Returns all the Maps in the MapGrid (order top left to bottom right).
std::vector<Map *> MapGrid::maps()
{
    std::vector<Map*> resultantMaps;
    for (Map* map:maps_){
        if (map != nullptr){
            resultantMaps.push_back(map);
        }
    }
    return resultantMaps;
}
