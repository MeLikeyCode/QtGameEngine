#include "ECMoveByKeyboardPerspective.h"
#include "Map.h"
#include "QTimer"
#include "Sprite.h"
#include "Game.h"
#include <cassert>
#include "Utilities.h"

ECMoveByKeyboardPerspective::ECMoveByKeyboardPerspective(Entity* entity):
    entity_(entity),
    stepSize_(15),
    moveTimer_(new QTimer(this))
{
    // make sure passed in Entity is not nullptr
    assert(entity != nullptr);

    // connect timer to move step
    connect(moveTimer_,&QTimer::timeout,this,&ECMoveByKeyboardPerspective::moveStep_);
    moveTimer_->start(secondsToMs(frequency(stepSize_,entity_->speed())));
}

/// See ECPathMover::setStepSize().
void ECMoveByKeyboardPerspective::setStepSize(double stepSize)
{
    stepSize_ = stepSize;
}

/// See ECPathMover::stepSize().
double ECMoveByKeyboardPerspective::stepSize()
{
    return stepSize_;
}

void ECMoveByKeyboardPerspective::moveStep_()
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
        QLineF line(entity_->pos(),QPoint(0,0));
        line.setAngle(360-entity_->facingAngle());
        line.setLength(stepSize_);
        double newX = entity_->pos().x() + line.dx();
        double newY = entity_->pos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }
    }

    // move down if S is pressed
    if (sPressed){
        QLineF line(entity_->pos(),QPoint(0,0));
        line.setAngle(360-entity_->facingAngle());
        line.setLength(stepSize_);
        line.setAngle(line.angle()+180);
        double newX = entity_->pos().x() + line.dx();
        double newY = entity_->pos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }
    }

    // move left if A is pressed
    if (aPressed){
        QLineF line(entity_->pos(),QPoint(0,0));
        line.setAngle(360-entity_->facingAngle());
        line.setLength(stepSize_);
        line.setAngle(line.angle()+90);
        double newX = entity_->pos().x() + line.dx();
        double newY = entity_->pos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }

    }

    // move right if D is pressed
    if (dPressed){
        QLineF line(entity_->pos(),QPoint(0,0));
        line.setAngle(360-entity_->facingAngle());
        line.setLength(stepSize_);
        line.setAngle(line.angle()-90);
        double newX = entity_->pos().x() + line.dx();
        double newY = entity_->pos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);

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
