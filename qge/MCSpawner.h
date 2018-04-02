#pragma once

#include <QObject>
#include <QRectF>

#include "RandomGenerator.h"

class QTimer;

namespace qge{

class Map;

/// A map controller that will spawn entities randomly within some radius.
class MCSpawner: public QObject // inherits from QObject so can use signals/slots
{
    Q_OBJECT
public:

    enum class SpawnType {
        Spider
    };

    MCSpawner(Map* map, const QRectF& region, SpawnType spawnType, int max, double ratePerSec);

    void turnOn();
    void turnOff();

public slots:
    void onTimeout_();

private:
    Map* map_;
    QRectF region_;
    SpawnType spawnType_;
    int max_;
    double ratePerSec_;

    QTimer* timer_;
    RandomGenerator random_;
};

}
