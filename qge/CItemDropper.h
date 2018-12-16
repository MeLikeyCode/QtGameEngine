#pragma once

#include "Vendor.h"

namespace qge{

class Entity;

class CItemDropper : public QObject // inherits from QObject for signal/slot mechanism
{
    Q_OBJECT
public:
    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);

signals:

public slots:
    void onEntityDies_(Entity* entity);

private:
    std::unordered_set<Entity*> entities_;

};

}
