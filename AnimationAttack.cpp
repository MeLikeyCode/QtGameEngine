#include "AnimationAttack.h"
#include "Sound.h"
#include "Slot.h"
#include "EntitySprite.h"

AnimationAttack::AnimationAttack(std::string animationToPlayOnAttack, int damage):
    animationToPlayOnAttack_(animationToPlayOnAttack),
    damage_(damage)
{
    soundEffect_ = new Sound("qrc:/resources/sounds/axe.wav");
}

void AnimationAttack::attack(QPointF position)
{
    // play animation
    Entity* owner = slotEquippedIn()->owner();
    EntitySprite* ownersSprite = owner->sprite();
    ownersSprite->play(animationToPlayOnAttack_,1,20);

    // listen to when the animation finishes
    connect(ownersSprite,&EntitySprite::animationFinished,this,&AnimationAttack::onOwnerAnimationFinished_);

    // listen to collisions
    connect(owner,&Entity::collided,this,&AnimationAttack::onOwnerCollides_);
}

/// Executed when the owner of the AnimationAttack MeleeWeapon collides with someone
/// while the attack animation is playing.
void AnimationAttack::onOwnerCollides_(Entity *owner, Entity *collidedWith)
{
    collidedWith->damage(this,damage_);
}

/// Executed when the owner has finished playing its attack animation.
/// Will stop looking for collisions between owner and other entities after this.
void AnimationAttack::onOwnerAnimationFinished_(EntitySprite *ownerSprite, std::string animation)
{
    Entity* owner = slotEquippedIn()->owner();
    EntitySprite* ownersSprite = owner->sprite();

    disconnect(owner,&Entity::collided,this,&AnimationAttack::onOwnerCollides_);
    disconnect(ownersSprite,&EntitySprite::animationFinished,this,&AnimationAttack::onOwnerAnimationFinished_);
}
