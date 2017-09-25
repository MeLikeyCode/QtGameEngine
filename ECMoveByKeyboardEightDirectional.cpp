#include "ECMoveByKeyboardEightDirectional.h"
#include "Map.h"
#include <QTimer>
#include <cassert>
#include "Game.h"
#include "Sprite.h"
#include "Utilities.h"
#include "EntitySprite.h"

ECMoveByKeyboardEightDirectional::ECMoveByKeyboardEightDirectional(Entity *entity):
    entity_(entity),
    stepSize_(15),
    moveTimer_(new QTimer(this))
{
    // make sure passed in Entity is not nullptr
    assert(entity != nullptr);

    // connect timer to move step
    connect(moveTimer_,&QTimer::timeout,this,&ECMoveByKeyboardEightDirectional::moveStep_);
    moveTimer_->start(secondsToMs(frequency(stepSize_,entity_->speed())));
}

/// See ECPathMover::setStepSize().
void ECMoveByKeyboardEightDirectional::setStepSize(double stepSize)
{
    stepSize_ = stepSize;
}

/// See ECPathMover::stepSize().
double ECMoveByKeyboardEightDirectional::stepSize()
{
    return stepSize_;
}

void ECMoveByKeyboardEightDirectional::playAnimIfHas_(std::string anim)
{
    if (entity_->sprite()->hasAnimation(anim)){
        entity_->sprite()->play(anim,-1,10);
    }
}

void ECMoveByKeyboardEightDirectional::moveStep_()
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

    // temporarly disable pathingmap (will be automatically reenabled when moved)
    std::vector<QRectF> entitysCellsAsRects = entity_->pathingMap().cellsAsRects();
    std::vector<QRectF> entitysFilledCellsAsRects;
    for (QRectF rect:entitysCellsAsRects){
        if (entity_->pathingMap().filled(rect)){
            // shift it and add it to filled collection
            QPointF entitysPos = entity_->pos();
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

    // find out which keys are pressed during this step
    bool wPressed = entitysGame->keysPressed().count(Qt::Key_W);
    bool sPressed = entitysGame->keysPressed().count(Qt::Key_S);
    bool aPressed = entitysGame->keysPressed().count(Qt::Key_A);
    bool dPressed = entitysGame->keysPressed().count(Qt::Key_D);

    // move up if W is pressed
    if (wPressed){
        // find newPt to move to
        double newX = entity_->pos().x();
        double newY = entity_->pos().y() - stepSize_;
        QPointF newPt(newX,newY);

        // move if the new location is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt); // set pos
            playAnimIfHas_("walk"); // ensure animation is playing
        }
    }

    // move down if S is pressed
    if (sPressed){
        double newX = entity_->pos().x();
        double newY = entity_->pos().y() + stepSize_;
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);
            playAnimIfHas_("walk");
        }
    }

    // move left if A is pressed
    if (aPressed){
        double newX = entity_->pos().x() - stepSize_;
        double newY = entity_->pos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);
            playAnimIfHas_("walk");
        }
    }

    // move right if D is pressed
    if (dPressed){
        double newX = entity_->pos().x() + stepSize_;
        double newY = entity_->pos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPos(newPt);
            playAnimIfHas_("walk");
        }
    }

    // if none of the keys are pressed, play stand animation
    if (!wPressed && !aPressed && !sPressed && !dPressed){
        playAnimIfHas_("stand");
    }
}
