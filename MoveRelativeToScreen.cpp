#include "MoveRelativeToScreen.h"
#include "Entity.h"
#include "Map.h"
#include "Game.h"
#include "MapGrid.h"
#include "Sprite.h"

/// Sets the entity to be moved.
void MoveRealtiveToScreen::setEntity(Entity *entity)
{
    entity_ = entity;
}

/// Moves the entity relative to the screen.
void MoveRealtiveToScreen::moveStep()
{
    // temporarly disable pathingmap (will be automatically reenabled when moved)
    QPointF entitysPos = entity_->map()->cellToPoint(entity_->cellPos());

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
        std::vector<Node> intersectedCells = entity_->map()->pathingMap().cells(rect);
        for (Node cell:intersectedCells){
            entity_->map()->pathingMap().unfill(cell);
        }
    }

    // rotate towards mouse pos
    entity_->rotateTo(entity_->map()->getMousePosition());

    // find out which keys are pressed during this step
    bool wPressed = entity_->map()->game()->keysPressed().count(Qt::Key_W);
    bool sPressed = entity_->map()->game()->keysPressed().count(Qt::Key_S);
    bool aPressed = entity_->map()->game()->keysPressed().count(Qt::Key_A);
    bool dPressed = entity_->map()->game()->keysPressed().count(Qt::Key_D);

    // move up if W is pressed
    if (wPressed){        
        // find newPt to move to
        double newX = entity_->pointPos().x();
        double newY = entity_->pointPos().y() - entity_->stepSize();
        QPointF newPt(newX,newY);

        // move if the new location is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }

        // MUSTDO: test, remove load next map if far up
        if (entity_->pointY() < 25 ){
            Node currentMapPos = entity_->map()->game()->mapGrid()->positionOf(entity_->map());
            Map* nextMap = entity_->map()->game()->mapGrid()->mapAt(currentMapPos.x(),currentMapPos.y() - 1);
            if(nextMap){
                entity_->map()->game()->setCurrentMap(nextMap);
                entity_->setPointY(nextMap->height() - 200);
                nextMap->addEntity(entity_->map()->game()->player());
            }
        }

    }

    // move down if S is pressed
    if (sPressed){
        double newX = entity_->pointPos().x();
        double newY = entity_->pointPos().y() + entity_->stepSize();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }

        // MUSTDO: test, remove load next map if far up
        if (entity_->pointY() > entity_->map()->height() - 150 ){
            Node currentMapPos = entity_->map()->game()->mapGrid()->positionOf(entity_->map());
            Map* nextMap = entity_->map()->game()->mapGrid()->mapAt(currentMapPos.x(),currentMapPos.y() + 1);
            if(nextMap){
                entity_->map()->game()->setCurrentMap(nextMap);
                entity_->setPointY(150);
                nextMap->addEntity(entity_->map()->game()->player());
            }
        }
    }

    // move left if A is pressed
    if (aPressed){
        double newX = entity_->pointPos().x() - entity_->stepSize();
        double newY = entity_->pointPos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }

        // MUSTDO: test, remove load next map if far enough
        if (entity_->pointX() < 150 ){
            Node currentMapPos = entity_->map()->game()->mapGrid()->positionOf(entity_->map());
            Map* nextMap = entity_->map()->game()->mapGrid()->mapAt(currentMapPos.x() - 1,currentMapPos.y());
            if(nextMap){
                entity_->map()->game()->setCurrentMap(nextMap);
                entity_->setPointX(nextMap->width() - 200);
                nextMap->addEntity(entity_->map()->game()->player());
            }
        }

    }

    // move right if D is pressed
    if (dPressed){
        double newX = entity_->pointPos().x() + entity_->stepSize();
        double newY = entity_->pointPos().y();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity_->canFit(newPt)){
            entity_->setPointPos(newPt);

            // if the walk animation isn't playing already, play it.
            if (entity_->sprite()->playingAnimation() != std::string("walk")){
                entity_->sprite()->play("walk",-1,100);
            }
        }

        // MUSTDO: test, remove load next map if far up
        if (entity_->pointX() > entity_->map()->width() - 200 ){
            Node currentMapPos = entity_->map()->game()->mapGrid()->positionOf(entity_->map());
            Map* nextMap = entity_->map()->game()->mapGrid()->mapAt(currentMapPos.x() + 1,currentMapPos.y());
            if(nextMap){
                entity_->map()->game()->setCurrentMap(nextMap);
                entity_->setPointX(150);
                nextMap->addEntity(entity_->map()->game()->player());
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
