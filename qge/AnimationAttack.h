#pragma once

#include "Vendor.h"

#include "MeleeWeapon.h"

namespace qge{

class PositionalSound;

/// A MeleeWeapon that on attack() will cause a specified animation of the owner to be played.
/// Additionally, all entities that are within a specified arc will be damaged by a specified amount.
/// @author Abdullah Aghazadah
/// @date 11/3/17
class AnimationAttack : public MeleeWeapon
{
    Q_OBJECT
public:
    AnimationAttack(const std::string& animationToPlayOnAttack,
                    const std::string& soundToPlayOnAttack,
                    int damage, double arcRange, double arcAngle);
    void attack(QPointF position);

public slots:
    void onFrameSwitched_(EntitySprite* ownersSprite, int fromFrameNum, int toFrameNum);
    void onOwnerAnimationFinished_(EntitySprite* ownerSprite, std::string animation);
private:
    std::string animationToPlayOnAttack_;
    std::string soundToPlayOnAttack_;
    int damage_;
    double archRange_;
    double arcAngle_;

    PositionalSound* soundEffect_;
    bool alreadyAttacking_;
    int attackingFrameNumber_; // the frame number to actually attack on (create arc and damage entities)
};

}
