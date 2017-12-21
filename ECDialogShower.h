#ifndef ECDIALOGSHOWER_H
#define ECDIALOGSHOWER_H

#include <unordered_set>
#include <QPointer>

#include "DialogGui.h"

class Entity;

#include "EntityController.h"


class ECDialogShower : public EntityController
{
public:
    ECDialogShower(Entity* controlledEntity, DialogGui* gui);

    void addEntityOfInterest(Entity* entity);
    void setDistance(double distance);
    double distance() const;

public slots:
    void onEntityMoved_();

private:
    double distance_;
    QPointer<DialogGui> gui_;
    std::unordered_set<QPointer<Entity>> entitiesOfInterest_;
};

#endif // ECDIALOGSHOWER_H
