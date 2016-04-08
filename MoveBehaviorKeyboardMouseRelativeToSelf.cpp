#include "MoveBehaviorKeyboardMouseRelativeToSelf.h"

#include "DynamicEntity.h"
#include "Map.h"
#include "Game.h"

MoveBehaviorKeyboardMouseRelativeToSelf::MoveBehaviorKeyboardMouseRelativeToSelf(DynamicEntity *entity):
    MoveBehavior(entity)
{

}

/// Executed whenever the DynamicEntity is told to move. Will see
/// which keys are pressed and move accordingly.
void MoveBehaviorKeyboardMouseRelativeToSelf::moveStep()
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
        QLineF line(entity->pointPos(),QPoint(0,0));
        line.setAngle(360-entity->facingAngle());
        line.setLength(entity->stepSize());
        double newX = entity->pointPos().x() + line.dx();
        double newY = entity->pointPos().y() + line.dy();
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
        QLineF line(entity->pointPos(),QPoint(0,0));
        line.setAngle(360-entity->facingAngle());
        line.setLength(entity->stepSize());
        line.setAngle(line.angle()+180);
        double newX = entity->pointPos().x() + line.dx();
        double newY = entity->pointPos().y() + line.dy();
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
        QLineF line(entity->pointPos(),QPoint(0,0));
        line.setAngle(360-entity->facingAngle());
        line.setLength(entity->stepSize());
        line.setAngle(line.angle()+90);
        double newX = entity->pointPos().x() + line.dx();
        double newY = entity->pointPos().y() + line.dy();
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
        QLineF line(entity->pointPos(),QPoint(0,0));
        line.setAngle(360-entity->facingAngle());
        line.setLength(entity->stepSize());
        line.setAngle(line.angle()-90);
        double newX = entity->pointPos().x() + line.dx();
        double newY = entity->pointPos().y() + line.dy();
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
