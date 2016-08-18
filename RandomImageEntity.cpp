#include "RandomImageEntity.h"
#include "Sprite.h"
#include "stdlib.h" // rand() and srand()
#include "time.h" // time(0) -> current time in ms

RandomImageEntity::RandomImageEntity(std::vector<QPixmap> pixmaps, const PathingMap &pathingMap)
{
    // TODO: lots of duplicate code b/w this and below ctor, factor out

    // approach:
    // - pick a random pixmap from the pixmaps
    // - set the Entity's sprite as that pixmap

    // seed
    srand(time(nullptr));

    // pick random frame index
    int randIndex = rand() % pixmaps.size(); // 0 to one less than num of pixmaps

    // set the sprites pixmap to that pixmap
    sprite()->setPixmap(pixmaps[randIndex]);

    double pathingMapX = pixmaps[0].width() / 2.0 - pathingMap.cellSize() / 2.0;
    double pathingMapY = pixmaps[0].height() / 2.0 - pathingMap.cellSize() / 2.0;

    setPathingMap(pathingMap,QPointF(pathingMapX,pathingMapY));
}

RandomImageEntity::RandomImageEntity(std::string resourceFolderPath, std::string imagePrefix, int numOfImages, const PathingMap &pm)
{
    std::vector<QPixmap> pixmaps;
    for (int i = 0, n = numOfImages; i < n; ++i){
        std::string fullPath = resourceFolderPath + "/" + imagePrefix + std::to_string(i) + ".png";
        pixmaps.push_back(QPixmap(fullPath.c_str()));
    }

    // pick random frame index
    int randIndex = rand() % pixmaps.size(); // 0 to one less than num of pixmaps

    // set the sprites pixmap to that pixmap
    sprite()->setPixmap(pixmaps[randIndex]);

    double pathingMapX = pixmaps[0].width() / 2.0 - pm.cellSize() / 2.0;
    double pathingMapY = pixmaps[0].height() / 2.0 - pm.cellSize() / 2.0;

    setPathingMap(pm,QPointF(pathingMapX,pathingMapY));
}
