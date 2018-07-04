#include "ECGuiShower.h"

#include "QtUtilities.h"
#include "Map.h"
#include "Game.h"
#include "STLWrappers.h"
#include "Gui.h"

using namespace qge;

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
/// entities of interest) moves. If the distance between *any* of the entities
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

    // lets clear dead entities of interst
    for (Entity* e:entitiesOfInterest_){
        if (e == nullptr)
            entitiesOfInterest_.erase(e);
            //STLWrappers::remove(entitiesOfInterest_,e); // doesn't work for some reason!
            //STLWrappers::remove(entitiesOfInterest_,QPointer<Entity>(e)); // have to do this! why!? TODO: potential improvement in STLWrappers library!
    }

    // set up variables
    double threshold = distance();
    Map* entitysMap = ec->map();
    Game* entitysGame = nullptr;
    if (entitysMap){
        entitysGame = entitysMap->game();
    }

    Entity* entityWithinRange = nullptr; // the *first* entity within range
    for (Entity* e:entitiesOfInterest_){
        if (QtUtils::distance(e->pos(),ec->pos()) < threshold && e->map() == entitysMap){
            entityWithinRange = e;
        }
    }

    // if someone met the threshold (i.e. is in range), make sure gui is added
    if (entityWithinRange){
        if (entitysGame){
            if (!entitysGame->containsGui(gui_)){
                entitysGame->addGui(gui_);
                emit guiShown(this,entityWithinRange);
            }
        }
    }
    // otherwise, make sure its removed
    else{
        if (entitysGame){
            if (entitysGame->containsGui(gui_)){
                entitysGame->removeGui(gui_.data());
                emit guiHidden(this);
            }
        }
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
