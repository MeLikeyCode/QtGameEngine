#ifndef RANDOMIMAGEENTITY_H
#define RANDOMIMAGEENTITY_H

#include "PathingMap.h"
#include "Entity.h"
#include <vector>

/// An Entity whos displayed image is randomly picked from one of the specified pixmaps.
/// The Entity's PathingMap stays the same, no matter which image is randomly picked.
/// This class is useful for generating eye-candy that needs a little variation in imagery,
/// such as trees, bushes, rocks, etc...
/// @author Abdullah Aghazadah
/// @date 8-16-16
class RandomImageEntity : public Entity
{
public:
    RandomImageEntity(std::vector<QPixmap> pixmaps, const PathingMap& pathingMap);
    RandomImageEntity(std::string resourceFolderPath, std::string imagePrefix, int numOfImages, const PathingMap& pm);
};

#endif // RANDOMIMAGEENTITY_H
