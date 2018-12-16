#include "MapGrid.h"

#include "Node.h"

using namespace qge;

MapGrid::MapGrid(int numMapsHorizontally, int numMapsVertically):
    pointerGrid_(numMapsHorizontally,numMapsVertically)
{
    // empty
}

/// Sets the specified Map in the specified position in the MapGrid.
/// @param map The map to set at the specified position. Can be nullptr.
void MapGrid::setMapAtPos(Map *map, int xPos, int yPos)
{
    pointerGrid_.setPointerAtPos(xPos,yPos,map);
}

/// Sets the specified Map in the specified position in the MapGrid.
/// @param map The Map to set at the specified position. Can be nullptr.
void MapGrid::setMapAtPos(Map *map, const Node &atPos)
{
    setMapAtPos(map,atPos.x(),atPos.y());
}

/// Returns the Map at the specified position in the MapGrid.
/// If there is no Map at the specified position, returns null.
/// If the position is outside the bounds of the MapGrid, will return null because
/// there is no Map there!
Map *MapGrid::mapAt(int xPos, int yPos) const
{
    return pointerGrid_.pointerAt(xPos,yPos);
}

/// Returns true if the MapGrid contains the specified map.
bool MapGrid::contains(Map *map) const
{
    return pointerGrid_.contains(map);
}

/// Returns the position of the specified map in the MapGrid.
/// If the specified map is not in the map grid, will throw.
/// Use contains(Map*) to check if the map is in the MapGrid before calling
/// this function.
Node MapGrid::positionOf(Map *map) const
{
    return pointerGrid_.positionOf(map);
}

/// Returns a vector of all the Maps in the MapGrid (order - left to right, top to bottom).
std::vector<Map *> MapGrid::maps() const
{
    return pointerGrid_.pointers();
}
