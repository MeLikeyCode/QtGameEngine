#pragma once

#include <string>

#include "MeleeWeapon.h"

class Sound;

/// A MeleeWeapon that on attack() will cause a specified animation of the owner to be played.
/// During this time, if there is a collision between the owner and another entity, the
/// other entity will get damanged by the specified amount.
/// @author Abdullah Aghazadah
/// @date 11/3/17
class AnimationAttack : public MeleeWeapon
{
    Q_OBJECT
public:
    AnimationAttack(std::string animationToPlayOnAttack, int damage);
    void attack(QPointF position);

public slots:
    void onOwnerCollides_(Entity* owner, Entity* collidedWith);
    void onOwnerAnimationFinished_(EntitySprite* ownerSprite, std::string animation);
private:
    Sound* soundEffect_;
    std::string animationToPlayOnAttack_;
    int damage_;
};

