#pragma once

#include "Vendor.h"

#include "PathingMap.h"
#include "Entity.h"

namespace qge{

/// An Entity whos displayed image is randomly picked from a set of images in a folder.
/// No matter which image is picked, the same pathing map that is specified will be used for the entity.
/// This class is useful for generating eye-candy that needs a little variation in imagery,
/// such as trees, bushes, rocks, etc...
/// @author Abdullah Aghazadah
/// @date 8-16-16
class RandomImageEntity : public Entity
{
public:
    RandomImageEntity(std::string resourceFolderPath, std::string imagePrefix, int numOfImages, PathingMap& pathingMap);
};

}
