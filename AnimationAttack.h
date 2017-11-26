#pragma once

#include <string>

#include "MeleeWeapon.h"

class Sound;

/// A MeleeWeapon that on attack() will cause a specified animation of the owner to be played.
/// Additionally, all entities that are within a specified arc will be damaged by a specified amount.
/// @author Abdullah Aghazadah
/// @date 11/3/17
class AnimationAttack : public MeleeWeapon
{
    Q_OBJECT
public:
    AnimationAttack(std::string animationToPlayOnAttack, int damage, double arcRange, double arcAngle);
    void attack(QPointF position);

public slots:
    void onFrameSwitched_(EntitySprite* ownersSprite, int fromFrameNum, int toFrameNum);
    void onOwnerAnimationFinished_(EntitySprite* ownerSprite, std::string animation);
private:
    std::string animationToPlayOnAttack_;
    int damage_;
    double archRange_;
    double arcAngle_;

    Sound* soundEffect_;
    bool alreadyAttacking_;
    int attackingFrameNumber_; // the frame number to actually attack (create arc and damage entities)
};

