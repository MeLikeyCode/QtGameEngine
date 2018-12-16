#include "ECKeyboardMoverPerspective.h"

#include "Map.h"
#include "QTimer"
#include "Utilities.h"
#include "Sprite.h"
#include "Game.h"
#include "EntitySprite.h"

using namespace qge;

ECKeyboardMoverPerspective::ECKeyboardMoverPerspective(Entity* entity):
    EntityController(entity),
    stepSize_(15),
    moveTimer_(new QTimer(this))
{
    // make sure passed in Entity is not nullptr
    assert(entity != nullptr);

    // connect timer to move step
    connect(moveTimer_,&QTimer::timeout,this,&ECKeyboardMoverPerspective::moveStep_);
    moveTimer_->start(secondsToMs(frequency(stepSize_,entityControlled()->speed())));
}

/// See ECPathMover::setStepSize().
void ECKeyboardMoverPerspective::setStepSize(double stepSize)
{
    stepSize_ = stepSize;
}

/// See ECPathMover::stepSize().
double ECKeyboardMoverPerspective::stepSize()
{
    return stepSize_;
}

void ECKeyboardMoverPerspective::moveStep_()
{
    // if the entity has been destroyed, stop
    Entity* entity = entityControlled();
    if (entity == nullptr){
        moveTimer_->disconnect();
        return;
    }

    // if currently not in a Map, do nothing
    Map* entitysMap = entity->map();
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
        QLineF line(entity->pos(),QPoint(0,0));
        line.setAngle(360-entity->facingAngle());
        line.setLength(stepSize_);
        double newX = entity->pos().x() + line.dx();
        double newY = entity->pos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity->canFit(newPt)){
            entity->setPos(newPt);
            playAnimIfExistsAndNoOtherPlaying_("walk");
        }
    }

    // move down if S is pressed
    if (sPressed){
        QLineF line(entity->pos(),QPoint(0,0));
        line.setAngle(360-entity->facingAngle());
        line.setLength(stepSize_);
        line.setAngle(line.angle()+180);
        double newX = entity->pos().x() + line.dx();
        double newY = entity->pos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity->canFit(newPt)){
            entity->setPos(newPt);
            playAnimIfExistsAndNoOtherPlaying_("walk");
        }
    }

    // move left if A is pressed
    if (aPressed){
        QLineF line(entity->pos(),QPoint(0,0));
        line.setAngle(360-entity->facingAngle());
        line.setLength(stepSize_);
        line.setAngle(line.angle()+90);
        double newX = entity->pos().x() + line.dx();
        double newY = entity->pos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity->canFit(newPt)){
            entity->setPos(newPt);
            playAnimIfExistsAndNoOtherPlaying_("walk");
        }

    }

    // move right if D is pressed
    if (dPressed){
        QLineF line(entity->pos(),QPoint(0,0));
        line.setAngle(360-entity->facingAngle());
        line.setLength(stepSize_);
        line.setAngle(line.angle()-90);
        double newX = entity->pos().x() + line.dx();
        double newY = entity->pos().y() + line.dy();
        QPointF newPt(newX,newY);

        // move if the newPt is free
        if (entity->canFit(newPt)){
            entity->setPos(newPt);
            playAnimIfExistsAndNoOtherPlaying_("walk");
        }

    }

    // if none of the keys are pressed, play stand animation
    if (!wPressed && !aPressed && !sPressed && !dPressed){
        playAnimIfExistsAndNoOtherPlaying_("stand");
    }
}

void ECKeyboardMoverPerspective::playAnimIfExistsAndNoOtherPlaying_(std::string anim)
{
    Entity* entity = entityControlled();
    EntitySprite* entitysSprite = entity->sprite();
    std::string playingAnim = entitysSprite->playingAnimation().name();
    if (entitysSprite->hasAnimation(anim) && (playingAnim == "" || playingAnim == "stand" || playingAnim == "walk"))
        entity->sprite()->play(anim,-1,10,0);
}
