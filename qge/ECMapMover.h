#pragma once

#include "Vendor.h"

#include "EntityController.h"
#include "Entity.h"

class QPointF;

namespace qge{

/// An entity controller (TODO: add doc link) that makes it such that when the
/// controlled entity touches the border of it's map, it will go to the next map
/// in the MapGrid in that direction.
///
/// Example usage:
/// ==============
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// ECMoveToNextMap* c = new ECMoveToNextMap(entity);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// Thats it. From then on whenever the entity's position is set near the border
/// of its map, it will move to the next map in the MapGrid in that direction.
class ECMapMover: public EntityController
{
    Q_OBJECT
public:
    ECMapMover(Entity* entity);

    void setBorderThreshold(double threshold);
    double borderThreshold();
public slots:
    void onEntityMoved(Entity* controlledEntity, QPointF fromPos, QPointF toPos);

private:
    double borderThreshold_;
};

}
