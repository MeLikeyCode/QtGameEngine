#include "RandomImageEntity.h"

#include "Sprite.h"
#include "stdlib.h" // rand() and srand()
#include "time.h" // time(0) -> current time in ms
#include "TopDownSprite.h"
#include "Utilities.h"

using namespace qge;

/// Constructs an entity whos display image will be randomly chosen from a set of images in a folder.
/// @param resourceFolderPath folder with the images
/// @param imagePrefix the prefix of the images
/// @param numOfImages how many images of the specified prefix are in the folder (must start with number 0)
/// @param pathingMap the pathing map to use for the entity.
RandomImageEntity::RandomImageEntity(std::string resourceFolderPath, std::string imagePrefix, int numOfImages, PathingMap &pathingMap)
{
    std::vector<QPixmap> pixmaps;
    for (int i = 0, n = numOfImages; i < n; ++i){
        std::string fullPath = resourceFolderPath + "/" + imagePrefix + std::to_string(i) + ".png";
        pixmaps.push_back(QPixmap(fullPath.c_str()));
    }

    // pick random frame index
    int randIndex = randInt(0,pixmaps.size()-1);

    // set the sprites pixmap to that pixmap
    TopDownSprite* spr = new TopDownSprite(pixmaps[randIndex]);
    setSprite(spr);

    // set pathing map
    setPathingMap(pathingMap);

    setInvulnerable(true); // make random image entities invulnerable ( they are just for show for gods sake! :P)
}
