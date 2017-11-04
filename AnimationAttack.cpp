#include "AnimationAttack.h"
#include "Sound.h"
#include "Slot.h"
#include "EntitySprite.h"
#include "TopDownSprite.h"
#include "Map.h"
#include "Utilities.h"

AnimationAttack::AnimationAttack(std::string animationToPlayOnAttack, int damage):
    animationToPlayOnAttack_(animationToPlayOnAttack),
    damage_(damage),
    alreadyAttacking_(false)
{
    soundEffect_ = new Sound("qrc:/resources/sounds/axe.wav");
    setSprite(new TopDownSprite(QPixmap()));
}

void AnimationAttack::attack(QPointF position)
{
    // do nothing if were already attacking
    if (alreadyAttacking_)
        return;

    Entity* owner = slotEquippedIn()->owner();
    EntitySprite* ownersSprite = owner->sprite();

    // listen to when the animation finishes (so we know we are done attacking)
    connect(ownersSprite,&EntitySprite::animationFinished,this,&AnimationAttack::onOwnerAnimationFinished_);

    // play animation
    alreadyAttacking_ = true;
    ownersSprite->play(animationToPlayOnAttack_,1,10,0);

    // get everyone in arc and damage them
    Map* entitysMap = owner->map();
    QPolygonF poly;
    QPointF ownersPos = centerPos(owner);
    poly.append(ownersPos);
    QLineF line(ownersPos,QPointF(0,0));
    line.setAngle(owner->facingAngle() * -1);
    const static double LENGTH = 75;
    const static double DELTA = 30;
    line.setLength(LENGTH);
    line.setAngle(line.angle() + DELTA);
    poly.append(line.p2());
    line.setAngle(line.angle() - DELTA * 2);
    poly.append(line.p2());

    std::unordered_set<Entity*> entsInRegion = entitysMap->entities(poly);
    for (Entity* e:entsInRegion)
        this->damage(e,damage_);
}

/// Executed when the owner has finished playing its attack animation.
void AnimationAttack::onOwnerAnimationFinished_(EntitySprite *ownerSprite, std::string animation)
{
    Entity* owner = slotEquippedIn()->owner();
    EntitySprite* ownersSprite = owner->sprite();

    alreadyAttacking_ = false;
    disconnect(ownersSprite,&EntitySprite::animationFinished,this,&AnimationAttack::onOwnerAnimationFinished_);
}
