#include "AnimationAttack.h"

#include "PositionalSound.h"
#include "Slot.h"
#include "EntitySprite.h"
#include "TopDownSprite.h"
#include "Map.h"
#include "Utilities.h"

using namespace qge;

AnimationAttack::AnimationAttack(const std::string& animationToPlayOnAttack, const std::string& soundToPlayOnAttack, int damage, double arcRange, double arcAngle):
    animationToPlayOnAttack_(animationToPlayOnAttack),
    soundToPlayOnAttack_(soundToPlayOnAttack),
    damage_(damage),
    archRange_(arcRange),
    arcAngle_(arcAngle),
    alreadyAttacking_(false),
    attackingFrameNumber_(0),
    soundEffect_(nullptr)
{
    setSprite(new TopDownSprite(QPixmap())); // empty sprite
    setIcon(QPixmap(":/resources/graphics/misc/axeicon.png"));
}

void AnimationAttack::attack(QPointF position)
{
    // do nothing if were already attacking
    if (alreadyAttacking_)
        return;

    // assert assumptions
    Entity* owner = slotEquippedIn()->owner();
    assert(owner != nullptr);
    Map* ownersMap = owner->map();
    assert(ownersMap != nullptr);
    EntitySprite* ownersSprite = owner->sprite();
    assert(ownersSprite != nullptr);

    // play sound
    soundEffect_ = new PositionalSound(ownersMap, soundToPlayOnAttack_,QPointF());
    soundEffect_->setPos(owner->pos());
    soundEffect_->play(1);

    // listen to when the animation reaches nth frame (when we'll apply the damage)
    connect(ownersSprite,&EntitySprite::frameSwitched,this,&AnimationAttack::onFrameSwitched_);

    // listen to when the animation finishes (so we know we are done attacking)
    connect(ownersSprite,&EntitySprite::animationFinished,this,&AnimationAttack::onOwnerAnimationFinished_);

    // play animation
    alreadyAttacking_ = true;
    ownersSprite->playThenGoBackToOldAnimation(animationToPlayOnAttack_,1,10,0);
}

/// Executed when the owner's sprite's frames are being switched.
/// We will see if we are on the nth frame and if so we will damage entities in an arc and then stop listening to this event.
void AnimationAttack::onFrameSwitched_(EntitySprite *ownersSprite, int fromFrameNum, int toFrameNum)
{
    if (toFrameNum == attackingFrameNumber_){
        Entity* owner = slotEquippedIn()->owner();
        EntitySprite* ownersSprite = owner->sprite();

        // get everyone in arc and damage them
        Map* entitysMap = owner->map();
        QPolygonF poly;
        QPointF ownersPos = owner->pos();
        poly.append(ownersPos);
        QLineF line(ownersPos,QPointF(0,0));
        line.setAngle(owner->facingAngle() * -1);
        line.setLength(archRange_);
        line.setAngle(line.angle() + arcAngle_/2);
        poly.append(line.p2());
        line.setAngle(line.angle() - arcAngle_);
        poly.append(line.p2());

        //owner->map()->scene()->addPolygon(poly); // DEBUG, enable this to visualize attack area

        std::unordered_set<Entity*> entsInRegion = entitysMap->entities(poly);
        for (Entity* e:entsInRegion){
            if (e != this && e != owner) { // don't hurt self
                owner->damageEnemy(e,damage_);
            }
        }

        // stop listening to frame switched (will relisten on new attack)
        disconnect(ownersSprite,&EntitySprite::frameSwitched,this,&AnimationAttack::onFrameSwitched_);
    }
}

/// Executed when the owner has finished playing its attack animation.
void AnimationAttack::onOwnerAnimationFinished_(EntitySprite *ownerSprite, std::string animation)
{
    Entity* owner = slotEquippedIn()->owner();
    EntitySprite* ownersSprite = owner->sprite();

    alreadyAttacking_ = false;
    disconnect(ownersSprite,&EntitySprite::animationFinished,this,&AnimationAttack::onOwnerAnimationFinished_);
}
