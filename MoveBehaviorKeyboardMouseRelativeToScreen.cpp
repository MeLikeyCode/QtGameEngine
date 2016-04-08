#include "MoveBehaviorKeyboardMouseRelativeToScreen.h"
#include "DynamicEntity.h"
#include "Map.h"
#include "Game.h"

MoveBehaviorKeyboardMouseRelativeToScreen::MoveBehaviorKeyboardMouseRelativeToScreen(DynamicEntity *entity):
    MoveBehavior(entity)
{

}

/// Executed whenever the entity is told to move. Will check if a movement
/// key is pressed, if so, will move appropriately. Additionally, will
/// rotate to face the mouse.
void MoveBehaviorKeyboardMouseRelativeToScreen::moveStep()
{
    DynamicEntity* entity = getEntity();
    Map* map = entity->map();
    set<int> keysPressed = map->game()->keysPressed();

    // rotate towards mouse pos
    entity->rotateTo(map->getMousePosition());

    bool wPressed = keysPressed.count(Qt::Key_W);
    bool sPressed = keysPressed.count(Qt::Key_S);
    bool aPressed = keysPressed.count(Qt::Key_A);
    bool dPressed = keysPressed.count(Qt::Key_D);

    // move up if W is pressed
    if (wPressed){
        // find newPt to move to
        double newX = entity->pointPos().x();
        double newY = entity->pointPos().y() - entity->stepSize();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity->canFit(newPt)){
            entity->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity->sprite()->playingAnimation() != std::string("walk")){
                entity->sprite()->play("walk",-1,100);
            }
        }

    }

    // move down if S is pressed
    if (sPressed){
        double newX = entity->pointPos().x();
        double newY = entity->pointPos().y() + entity->stepSize();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity->canFit(newPt)){
            entity->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity->sprite()->playingAnimation() != std::string("walk")){
                entity->sprite()->play("walk",-1,100);
            }
        }
    }

    // move left if A is pressed
    if (aPressed){
        double newX = entity->pointPos().x() - entity->stepSize();
        double newY = entity->pointPos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity->canFit(newPt)){
            entity->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity->sprite()->playingAnimation() != std::string("walk")){
                entity->sprite()->play("walk",-1,100);
            }
        }

    }

    // move right if D is pressed
    if (dPressed){
        double newX = entity->pointPos().x() + entity->stepSize();
        double newY = entity->pointPos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity->canFit(newPt)){
            entity->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity->sprite()->playingAnimation() != std::string("walk")){
                entity->sprite()->play("walk",-1,100);
            }
        }

    }

    // if none of the keys are pressed, play stand animation
    if (!wPressed && !aPressed && !sPressed && !dPressed){
        // only play if it isn't already playing
        if (entity->sprite()->playingAnimation() != std::string("stand")){
            entity->sprite()->play("stand",-1,100);
        }
    }
}
