#pragma once

#include <QtGlobal>
#include <vector>

class QPointF;

namespace qge{

/// This file contains a bunch of utility functions for the game engine.
/// @author Abdullah Aghazadah
/// @date 8-17-16
/// @file

class Map;
class AIEntity;
class Entity;

// convenience functions
void addRandomTrees(Map* mapToAddTreesTo, int numTreesToAdd, const std::string& treeNumber, int numImages);

// mathmatical
double frequency(double stepSize, double rate);
double secondsToMs(double seconds);
double msToSeconds(double milliseconds);
quint32 fastRandom();
double closestAngle(const std::vector<int>& allAngles, double targetAngle);
double randInt(int from, int to);
double randDouble(double from, double to);

}
