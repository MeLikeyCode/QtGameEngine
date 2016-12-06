#include "ECMoveInResponseToKeyboardRelativeToSelf.h"
#include "Map.h"
#include "QTimer"
#include "Sprite.h"
#include "Game.h"
#include <cassert>
#include "Utilities.h"

ECMoveInResponseToKeyboardRelativeToSelf::ECMoveInResponseToKeyboardRelativeToSelf(Entity* entity):
    entity_(entity),
    stepSize_(15),
    moveTimer_(new QTimer(this))
{
    // make sure passed in Entity is not nullptr
    assert(entity != nullptr);

    // connect timer to move step
    connect(moveTimer_,&QTimer::timeout,this,&ECMoveInResponseToKeyboardRelativeToSelf::moveStep_);
    moveTimer_->start(secondsToMs(frequency(stepSize_,entity_->speed())));
}

/// See ECPathMover::setStepSize().
void ECMoveInResponseToKeyboardRelativeToSelf::setStepSize(double stepSize)
{
    stepSize_ = stepSize;
}

/// See ECPathMover::stepSize().
double ECMoveInResponseToKeyboardRelativeToSelf::stepSize()
{
    return stepSize_;
}

void ECMoveInResponseToKeyboardRelativeToSelf::moveStep_()
{
    // if the entity has been destroyed, stop
    if (entity_.isNull()){
        moveTimer_->disconnect();
        return;
    }

    // if currently not in a Map, do nothing
    Map* entitysMap = entity_->map();
    if (entitysMap == nullptr)
        return;

    // if entitysMap is not in a Game, do noting
    Game* entitysGame = entitysMap->game();
    if (entitysGame == nullptr)
        return;

    bool wPressed = entitysGame->keysPressed().count(Qt::Key_W);
    bool sPressed = entitysGame->keysPressed().count(Qt::Key_S);
    bool aPressed = entitysGame->keysPressed().count(Qt::Key_A);
    bool dPressed = entitysGame->keysPressed().count(Qt::Key_D);

    // move up if W is pressed
    if (wPressed){
        // find newPt to move to
        QLineF line(entity_->pointPos(),QPoint(0,0));
        line.setAngle(360-entity_->facingAngle());
        line.setLength(stepSize_);
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
        line.setLength(stepSize_);
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
        line.setLength(stepSize_);
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
        line.setLength(stepSize_);
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
