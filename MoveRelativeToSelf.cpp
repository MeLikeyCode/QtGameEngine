#include "MoveRelativeToSelf.h"
#include "DynamicEntity.h"
#include "Map.h"
#include "Game.h"

/// Sets the entity to move.
void MoveRelativeToSelf::setEntity(DynamicEntity *entity)
{
    entity_ = entity;
}

/// Moves the entity one step closer relative to itself.
void MoveRelativeToSelf::moveStep()
{
    // rotate towards mouse pos
    entity_->rotateTo(entity_->map()->getMousePosition());

    bool wPressed = entity_->map()->game()->keysPressed().count(Qt::Key_W);
    bool sPressed = entity_->map()->game()->keysPressed().count(Qt::Key_S);
    bool aPressed = entity_->map()->game()->keysPressed().count(Qt::Key_A);
    bool dPressed = entity_->map()->game()->keysPressed().count(Qt::Key_D);

    // move up if W is pressed
    if (wPressed){
        // find newPt to move to
        QLineF line(entity_->pointPos(),QPoint(0,0));
        line.setAngle(360-entity_->facingAngle());
        line.setLength(entity_->stepSize());
        double newX = entity_->pointPos().x() + line.dx();
        double newY = entity_->pointPos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }

    }

    // move down if S is pressed
    if (sPressed){
        QLineF line(entity_->pointPos(),QPoint(0,0));
        line.setAngle(360-entity_->facingAngle());
        line.setLength(entity_->stepSize());
        line.setAngle(line.angle()+180);
        double newX = entity_->pointPos().x() + line.dx();
        double newY = entity_->pointPos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }
    }

    // move left if A is pressed
    if (aPressed){
        QLineF line(entity_->pointPos(),QPoint(0,0));
        line.setAngle(360-entity_->facingAngle());
        line.setLength(entity_->stepSize());
        line.setAngle(line.angle()+90);
        double newX = entity_->pointPos().x() + line.dx();
        double newY = entity_->pointPos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }

    }

    // move right if D is pressed
    if (dPressed){
        QLineF line(entity_->pointPos(),QPoint(0,0));
        line.setAngle(360-entity_->facingAngle());
        line.setLength(entity_->stepSize());
        line.setAngle(line.angle()-90);
        double newX = entity_->pointPos().x() + line.dx();
        double newY = entity_->pointPos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }

    }

    // if none of the keys are pressed, play stand animation
    if (!wPressed && !aPressed && !sPressed && !dPressed){
        // only play if it isn't already playing
        if (entity_->sprite()->playingAnimation() != std::string("stand")){
            entity_->sprite()->play("stand",-1,100);
        }
    }
}
