#ifndef UTILITIES_H
#define UTILITIES_H

#include <QtGlobal>
#include <vector>

/// This file contains a bunch of utility functions for the game engine.
/// @author Abdullah Aghazadah
/// @date 8-17-16
/// @file

class Map;
class QPointF;
class AIEntity;
class Entity;

// convenience functions
void addRandomTrees(Map* mapToAddTreesTo, int numTreesToAdd);
void addRandomRocks(Map* mapToAddRocksTo, int numRocksToAdd);
void addRandomBushes(Map* mapToAddBushesTo, int numBushesToAdd);

// mathmatical
double frequency(double stepSize, double rate);
double secondsToMs(double seconds);
double msToSeconds(double milliseconds);
quint32 fastRandom();
double closestAngle(const std::vector<int>& allAngles, double targetAngle);
double randInt(int from, int to);

#endif // UTILITIES_H
