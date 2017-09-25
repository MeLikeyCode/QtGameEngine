#include "ECMoveByKeyboardFourDirectional.h"
#include "Map.h"
#include <QTimer>
#include <cassert>
#include "Game.h"
#include "Sprite.h"
#include "Utilities.h"
#include "EntitySprite.h"

ECMoveByKeyboardFourDirectional::ECMoveByKeyboardFourDirectional(Entity *entity):
    entity_(entity),
    stepSize_(15),
    moveTimer_(new QTimer(this))
{
    // make sure passed in Entity is not nullptr
    assert(entity != nullptr);

    // connect timer to move step
    connect(moveTimer_,&QTimer::timeout,this,&ECMoveByKeyboardFourDirectional::moveStep_);
    moveTimer_->start(secondsToMs(frequency(stepSize_,entity_->speed())));
}

/// See ECPathMover::setStepSize().
void ECMoveByKeyboardFourDirectional::setStepSize(double stepSize)
{
    stepSize_ = stepSize;
}

/// See ECPathMover::stepSize().
double ECMoveByKeyboardFourDirectional::stepSize()
{
    return stepSize_;
}

void ECMoveByKeyboardFourDirectional::moveStep_()
{
    // this function is executed periodically when the EC needs to move the entity
    // - we will check to see which keys are currently pressed
    // - we will move the entity in the correct direction with the correct speed and play the correct animation

    // if the entity has been destroyed, stop
    if (entity_.isNull()){
        moveTimer_->disconnect();
        return;
    }

    // if the entity is currently not in a Map, do nothing
    // TODO: instead of polling, listen to when entiy enters a map (efficiency)
    Map* entitysMap = entity_->map();
    if (entitysMap == nullptr)
        return;

    // if entitysMap is not in a Game, do noting
    // TODO: instead of polling, listen to when the entiys map enters a game (efficiency)
    Game* entitysGame = entitysMap->game();
    if (entitysGame == nullptr)
        return;

    // temporarly disable entity's pathingmap (will be automatically reenabled when moved)
    QPointF entitysPos = entity_->pos();
    std::vector<QRectF> entitysCellsAsRects = entity_->pathingMap().cellsAsRects();
    std::vector<QRectF> entitysFilledCellsAsRects;
    for (QRectF rect:entitysCellsAsRects){
        if (entity_->pathingMap().filled(rect)){
            // shift it and add it to filled collection
            rect.moveTopLeft(QPointF(entitysPos.x() + rect.x(), entitysPos.y() + rect.y()));
            entitysFilledCellsAsRects.push_back(rect);
        }
    }
    for (QRectF rect:entitysFilledCellsAsRects){
        std::vector<Node> intersectedCells = entitysMap->pathingMap().cells(rect);
        for (Node cell:intersectedCells){
            entitysMap->pathingMap().unfill(cell);
        }
    }

    // find out which keys are pressed during this move step
    bool wPressed = entitysGame->keysPressed().count(Qt::Key_W);
    bool sPressed = entitysGame->keysPressed().count(Qt::Key_S);
    bool aPressed = entitysGame->keysPressed().count(Qt::Key_A);
    bool dPressed = entitysGame->keysPressed().count(Qt::Key_D);

    // move up if W is pressed
    if (wPressed){
        // find newPt to move to
        double newX = entity_->x();
        double newY = entity_->y() - stepSize_;
        QPointF newPt(newX,newY);

        // move if the new location is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);
            playAnimationIfItExists_("walk");
        }
        return;
    }

    // move down if S is pressed
    if (sPressed){
        double newX = entity_->pos().x();
        double newY = entity_->pos().y() + stepSize_;
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);
            playAnimationIfItExists_("walk");
        }
        return;
    }

    // move left if A is pressed
    if (aPressed){
        double newX = entity_->pos().x() - stepSize_;
        double newY = entity_->pos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);
            playAnimationIfItExists_("walk");
        }
        return;
    }

    // move right if D is pressed
    if (dPressed){
        double newX = entity_->pos().x() + stepSize_;
        double newY = entity_->pos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);
            playAnimationIfItExists_("walk");
        }
        return;
    }

    // if none of the keys are pressed, play stand animation at currently facing direction
    if (!wPressed && !aPressed && !sPressed && !dPressed){
        playAnimationIfItExists_("stand");
    }
}

/// Plays the specified animation of the entity_ only if the entity has that
/// animation, otherwise does nothing. This function exists solely to reduce
/// code duplication (b/c I was using this fragment of code in 4 places).
void ECMoveByKeyboardFourDirectional::playAnimationIfItExists_(std::string animation)
{
    if (entity_->sprite()->hasAnimation(animation))
        entity_->sprite()->play(animation,-1,10);
}
