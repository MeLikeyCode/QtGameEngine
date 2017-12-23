#pragma once

#include <unordered_set>
#include <QPointer>

#include "EntityController.h"

class Gui;
class Entity;

/// An EntityController that will pop up a specified gui when entities of interst
/// come within a specified distance of the controlled entity.
///
/// Example
/// =======
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// ECGuiShower* gs = new ECGuiShower(controlledEntity,guiToShow); // specify the controlled entity and the gui
/// gs->addEntityOfInterest(e1);
/// gs->addEntityOfInterest(e2); // specify the 'entities of interst'
/// gs->setDistance(100);
///
/// // anytime an 'entity of interest' (e1 or e2 in this case) come within 100 of controlledEntity, the gui 'guiToShow' will be shown.
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class ECGuiShower : public EntityController
{
public:
    ECGuiShower(Entity* controlledEntity, Gui* gui);

    void addEntityOfInterest(Entity* entity);
    void setDistance(double distance);
    double distance() const;

public slots:
    void onEntityMoved_();

private:
    double distance_;
    QPointer<Gui> gui_;
    std::unordered_set<QPointer<Entity>> entitiesOfInterest_;

    virtual void onControlledEntityDying(Entity* controlledEntity);
};
