#include "Axe.h"
#include "Sprite.h"
#include "Map.h"
#include <QGraphicsScene>
#include <QDebug> // TODO: remove
#include "Inventory.h"

Axe::Axe()
{
    // defaults swing parameters
    swingAngleEachStep_ = 5;
    swingFrequency_ = 19;
    numOfSwingStepsBackward_ = 9;
    numOfSwingStepsForward_ = 8;
    numOfCurrentSwingSteps_ = 0;
    swingingForwardPhase_ = false;
    swingingOutPhase_ = false;
    alreadySwinging_ = false;
    headingBackwardPhase_ = false;

    // default sprite
    spr_ = new Sprite();
    pm_ = QPixmap(":resources/graphics/weapons/axe.png");
    spr_->setPixmap(pm_);
    setSprite(spr_);

    // default tip (length and width/2)
    resetTip();

    // default attachment point
    QPointF pt;
    pt.setX(pm_.width()/3);
    pt.setY(pm_.height()/2);
    setAttachmentPoint(pt);

    timer_ = new QTimer(this);
}

/// The axe will start swinging.
void Axe::attack()
{
    // if its already swinging, don't do anything
    if (alreadySwinging_){
        return;
    }

    // set initial state variables
    numOfCurrentSwingSteps_ = 0;
    swingingForwardPhase_ = false;
    headingBackwardPhase_ = false;
    swingingOutPhase_ = true;
    alreadySwinging_ = true;


    // connect to swingStep()
    connect(timer_,SIGNAL(timeout()),this,SLOT(swingStep()));
    timer_->start(swingFrequency_);
}

void Axe::swingStep()
{
    // if in swinging out phase
    if (swingingOutPhase_ && numOfCurrentSwingSteps_ < numOfSwingStepsBackward_){
        setFacingAngle(facingAngle()-swingAngleEachStep_);
        numOfCurrentSwingSteps_++;
        return;
    }

    // if done with swinging out phase
    if (swingingOutPhase_ && numOfCurrentSwingSteps_ >= numOfSwingStepsBackward_){
        swingingOutPhase_ = false;
        swingingForwardPhase_ = true;
        numOfCurrentSwingSteps_ = 0;
    }

    // if swinging forward
    if (swingingForwardPhase_ && numOfCurrentSwingSteps_ < numOfSwingStepsForward_ + numOfSwingStepsBackward_){
        // swing forward
        setFacingAngle(facingAngle()+swingAngleEachStep_);
        numOfCurrentSwingSteps_++;

        // kill things (except its owner or any of the owner's children)
        Entity* theOwner = inventory()->entity();
        std::unordered_set<Entity*> collidingEntities = map()->entities(mapToMap(tip()));
        for (Entity* e: collidingEntities){
            if (e != this && e != theOwner && e->parent() != theOwner){
                map()->removeEntity(e);
                delete e;

                // move back
                // swingingForwardPhase_ = false;
                // headingBackwardPhase_ = true;
                // numOfCurrentSwingSteps_ = numOfSwingStepsForward_ - numOfCurrentSwingSteps_;
            }
        }

        return;  
    }

    // if done swinging forward
    if (swingingForwardPhase_ && numOfCurrentSwingSteps_ >= numOfSwingStepsForward_ + numOfSwingStepsBackward_){
        swingingForwardPhase_ = false;
        headingBackwardPhase_ = true;
        numOfCurrentSwingSteps_ = 0;
        return;
    }

    // if heading backward
    if (headingBackwardPhase_ && numOfCurrentSwingSteps_ < numOfSwingStepsForward_){
        setFacingAngle(facingAngle()-swingAngleEachStep_);
        numOfCurrentSwingSteps_++;
        return;
    }

    // if done heading backward
    if (headingBackwardPhase_ && numOfCurrentSwingSteps_ >= numOfSwingStepsForward_){
        timer_->disconnect();
        alreadySwinging_ = false;
    }
}
