#pragma once

#include "Vendor.h"

class QTimer;

namespace qge{

class Entity;
class Map;
class Bar;

class CHealthShower : public QObject
{
    Q_OBJECT // inherits from QObject for signals/slots
public:
    CHealthShower();

    void addEntity(Entity* entity);

public slots:
    void onEntityMoved_(Entity* sender, QPointF fromPos, QPointF toPos);
    void onEntityHealthChanged_(Entity* sender);
    void onEntityEntersMap_(Entity* sender, Map* mapEntered, Map* oldMap);
    void onEntityLeavesMap_(Entity* sender, Map* mapLeft);
    void onEntitysMapVisualized_(Map* mapVisualized);
    void onEntitysMapUnVisualized_(Map* mapUnVisualized);
    void onEntityDying_(Entity* sender);

    void onTick_();

private:
    std::unordered_set<Entity*> entities_;
    std::unordered_map<Entity*,Bar*> entityToBar_;
    QTimer* timer_;
};

}
