#pragma once

#include "Vendor.h"

#include "RandomGenerator.h"
#include "EntityCreator.h"

class QTimer;

namespace qge{

class Map;

/// A map controller that will spawn entities randomly within some region of a map.
/// The EntityCreator of the MCSpawner specifies what entity will be created.
/// When it is time to create an entity, MCSpawner will get an entity from its EntityCreator,
/// and then place that entity in a random spot in the map.
class MCSpawner: public QObject // inherits from QObject so can use signals/slots
{
    Q_OBJECT
public:
    MCSpawner(Map* map, const QRectF& region, int max, double ratePerSec, EntityCreator* entityCreator);

    void turnOn();
    void turnOff();

public slots:
    void onTimeout_();

private:
    Map* map_;
    QRectF region_;
    EntityCreator* entityCreator_;
    int max_;
    double ratePerSec_;

    QTimer* timer_;
    RandomGenerator random_;
};

}
