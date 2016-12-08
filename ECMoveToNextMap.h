#ifndef ECMOVETONEXTMAP_H
#define ECMOVETONEXTMAP_H

#include <QPointer>
#include "Entity.h"
#include <QObject>

class QPointF;

/// An entity controller (TODO: add doc link) that makes it such that when the
/// controlled entity touches the border of it's map, it will go to the next map
/// in the MapGrid in that direction.
///
/// Example usage:
/// ECMoveToNextMap* c = new ECMoveToNextMap(entity);
///
/// Thats it. From then on whenever the entity's position is set near the border
/// of its map, it will move to the next map in the MapGrid in that direction.
class ECMoveToNextMap: public QObject
{
    Q_OBJECT
public:
    ECMoveToNextMap(Entity* entity);

    void setBorderThreshold(double threshold);
    double borderThreshold();
public slots:
    void onEntityMoved(Entity* controlledEntity, QPointF fromPos, QPointF toPos);

private:
    QPointer<Entity> entity_;
    double borderThreshold_;
};

#endif // ECMOVETONEXTMAP_H
