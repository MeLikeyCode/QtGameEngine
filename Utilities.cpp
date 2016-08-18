#include "Map.h"
#include "RandomImageEntity.h"
#include "PathingMap.h"
#include <stdlib.h>
#include <time.h>

/// Adds the specified number of trees randomly scattered on the specified Map.
void addRandomTrees(Map *mapToAddTreesTo, int numTreesToAdd)
{
    srand(time(nullptr));

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
