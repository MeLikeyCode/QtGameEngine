/// This file defines Utilities.h
/// @author Abdullah Aghazadah
/// @date 8-17-16
/// @file

#include <stdlib.h>
#include <time.h>

#include "Map.h"
#include "RandomImageEntity.h"
#include "PathingMap.h"
#include <QtMath>


/// Adds the specified number of trees randomly scattered on the specified Map.
void addRandomTrees(Map *mapToAddTreesTo, int numTreesToAdd)
{
    for (int i = 0, n = numTreesToAdd; i < n; i++){
        PathingMap pathingMap(1,1,32);
        pathingMap.fill();

        RandomImageEntity* tree = new RandomImageEntity(":/resources/graphics/tree","tree",3,pathingMap);
        mapToAddTreesTo->addEntity(tree);

        double randXPos = rand() % mapToAddTreesTo->width() - 100;
        double randYPos = rand() % mapToAddTreesTo->height() - 100;
        tree->setPointPos(QPointF(randXPos,randYPos));
    }
}

/// Adds the specified number of rocks randomly scattered on the specified Map.
void addRandomRocks(Map *mapToAddRocksTo, int numRocksToAdd)
{
    for (int i = 0, n = numRocksToAdd; i < n; i++){
        PathingMap pathingMap(1,1,32);
        pathingMap.fill();

        RandomImageEntity* rock = new RandomImageEntity(":/resources/graphics/rock","rock",3,pathingMap);
        mapToAddRocksTo->addEntity(rock);

        double randXPos = rand() % mapToAddRocksTo->width() - 100;
        double randYPos = rand() % mapToAddRocksTo->height() - 100;
        rock->setPointPos(QPointF(randXPos,randYPos));
    }
}

/// Adds the specified number of bushes randomly scattered on the specified Map.
void addRandomBushes(Map *mapToAddBushesTo, int numBushesToAdd)
{
    for (int i = 0, n = numBushesToAdd; i < n; i++){
        PathingMap pathingMap(1,1,32);
        pathingMap.fill();

        RandomImageEntity* rock = new RandomImageEntity(":/resources/graphics/bush","bush",4,pathingMap);
        mapToAddBushesTo->addEntity(rock);

        double randXPos = rand() % mapToAddBushesTo->width() - 100;
        double randYPos = rand() % mapToAddBushesTo->height() - 100;
        rock->setPointPos(QPointF(randXPos,randYPos));
    }
}

/// Returns the distance between the two points.
double distance(QPointF p1, QPointF p2)
{
    double deltaX = p2.x() - p1.x();
    double deltaY = p2.y() - p1.y();
    return qAbs(qSqrt(qPow(deltaX,2) + qPow(deltaY,2)));
}
