#ifndef UTILITIES_H
#define UTILITIES_H

/// This file contains a bunch of utility functions for the game engine.
/// @author Abdullah Aghazadah
/// @date 8-17-16
/// @file

class Map;
class QPointF;
class AIEntity;

void addRandomTrees(Map* mapToAddTreesTo, int numTreesToAdd);
void addRandomRocks(Map* mapToAddRocksTo, int numRocksToAdd);
void addRandomBushes(Map* mapToAddBushesTo, int numBushesToAdd);
AIEntity* addAIEntity(Map* mapToAddTo, const QPointF& posToAddTo, int teamToAddTo);

double distance(QPointF p1, QPointF p2);

#endif // UTILITIES_H
