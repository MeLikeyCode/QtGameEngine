#include "ECGuiShower.h"

#include <QtUtilities.h>

#include "Map.h"
#include "Game.h"
#include "STLWrappers.h"
#include "Gui.h"

ECGuiShower::ECGuiShower(Entity *controlledEntity, Gui* gui):
    EntityController(controlledEntity),
    gui_(gui),
    distance_(150)
{
    connect(controlledEntity,&Entity::moved,this,&ECGuiShower::onEntityMoved_);
}

void ECGuiShower::addEntityOfInterest(Entity *entity)
{
    disconnect(entity,&Entity::moved,this,&ECGuiShower::onEntityMoved_); // prevent double connect
    connect(entity,&Entity::moved,this,&ECGuiShower::onEntityMoved_);
    entitiesOfInterest_.insert(QPointer<Entity>(entity));
}

void ECGuiShower::setDistance(double distance)
{
    distance_ = distance;
}

double ECGuiShower::distance() const
{
    return distance_;
}

/// Executed when any of the entities (the controlled entity or any of the
/// entities of interst) moves. If the distance between *any* of the entities
/// of interest and the controlled entity is less than the threshold distance,
/// will make sure the dialog gui is shown, otherwise will make sure its hidden.
void ECGuiShower::onEntityMoved_()
{
    // algorithmic approach:
    // - compute the distance between each entity of interest and the controlled entity
    // - if any of those distances is less than the threshold distance, make
    //   sure the dialog gui is visible, else, make sure its invisible

    // implementation appraoch:
    // - use a range based for each loop to compare the distance of each


    Entity* ec = entityControlled();

    // if the controlled entity is dead, do nothing (this EC will be deleted soon if it hasn't already so no need to disconnect)
    if (ec == nullptr)
        return;

//    // lets clear dead entities of interst
//    for (Entity* e:entitiesOfInterest_){
//        if (e == nullptr)
//            STLWrappers::remove(entitiesOfInterest_,e);
//    }

    // set up variables
    double threshold = distance();
    Map* entitysMap = ec->map();
    Game* entitysGame = nullptr;
    if (entitysMap){
        entitysGame = entitysMap->game();
    }

    bool thresholdMet = false;
    for (Entity* e:entitiesOfInterest_)
        if (QtUtils::distance(e->pos(),ec->pos()) < threshold)
            thresholdMet = true;

    // if threshold met, make sure gui is added
    if (thresholdMet){
        if (entitysGame){
            entitysGame->addGui(gui_);
        }
    }
    // otherwise, make sure its removed
    else{
        if (entitysGame)
            entitysGame->removeGui(gui_.data());
    }
}

/// Executed when the controlled entity dies.
/// Will make sure gui is hidden.
void ECGuiShower::onControlledEntityDying(Entity *controlledEntity)
{
    Entity* ec = entityControlled();
    Map* entitysMap = ec->map();
    if (entitysMap){
        Game* entitysGame = entitysMap->game();
        if (entitysGame)
            entitysGame->removeGui(gui_);
    }
}
