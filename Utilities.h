#ifndef UTILITIES_H
#define UTILITIES_H

/// This file contains a bunch of utility functions for the game engine.
/// @author Abdullah Aghazadah
/// @date 8-17-16
/// @file

class Map;

void addRandomTrees(Map* mapToAddTreesTo, int numTreesToAdd);
void addRandomRocks(Map* mapToAddRocksTo, int numRocksToAdd);
void addRandomBushes(Map* mapToAddBushesTo, int numBushesToAdd);

#endif // UTILITIES_H
