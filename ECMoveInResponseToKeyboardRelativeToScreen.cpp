#include "ECMoveInResponseToKeyboardRelativeToScreen.h"
#include "Map.h"
#include <QTimer>
#include <cassert>
#include "Game.h"
#include "Sprite.h"

ECMoveInResponseToKeyboardRelativeToScreen::ECMoveInResponseToKeyboardRelativeToScreen(Entity *entity):
    entity_(entity),
    stepSize_(15),
    stepFrequency_(40),
    moveTimer_(new QTimer(this))
{
    // make sure passed in Entity is not nullptr
    assert(entity != nullptr);

    // connect timer to move step
    connect(moveTimer_,&QTimer::timeout,this,&ECMoveInResponseToKeyboardRelativeToScreen::moveStep_);
    moveTimer_->start(stepFrequency_);
}

ECMoveInResponseToKeyboardRelativeToScreen::~ECMoveInResponseToKeyboardRelativeToScreen()
{
    // make sure timer disconnects
    // - it will (since its a child of this qobject)
}

void ECMoveInResponseToKeyboardRelativeToScreen::moveStep_()
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
    QPointF entitysPos = entity_->pointPos();

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

    // find out which keys are pressed during this step
    bool wPressed = entitysGame->keysPressed().count(Qt::Key_W);
    bool sPressed = entitysGame->keysPressed().count(Qt::Key_S);
    bool aPressed = entitysGame->keysPressed().count(Qt::Key_A);
    bool dPressed = entitysGame->keysPressed().count(Qt::Key_D);

    // move up if W is pressed
    if (wPressed){
        // find newPt to move to
        double newX = entity_->pointPos().x();
        double newY = entity_->pointPos().y() - stepSize_;
        QPointF newPt(newX,newY);

        // move if the new location is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")
                    && entity_->sprite()->hasAnimation("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }
    }

    // move down if S is pressed
    if (sPressed){
        double newX = entity_->pointPos().x();
        double newY = entity_->pointPos().y() + stepSize_;
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")
                    && entity_->sprite()->hasAnimation("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }
    }

    // move left if A is pressed
    if (aPressed){
        double newX = entity_->pointPos().x() - stepSize_;
        double newY = entity_->pointPos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")
                    && entity_->sprite()->hasAnimation("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }
    }

    // move right if D is pressed
    if (dPressed){
        double newX = entity_->pointPos().x() + stepSize_;
        double newY = entity_->pointPos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")
                    && entity_->sprite()->hasAnimation("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }
    }

    // if none of the keys are pressed, play stand animation
    if (!wPressed && !aPressed && !sPressed && !dPressed){
        // only play if it isn't already playing
        if (entity_->sprite()->playingAnimation() != std::string("stand")
                && entity_->sprite()->hasAnimation("stand")){
            entity_->sprite()->play("stand",-1,100);
        }
    }
}
