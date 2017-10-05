#include "AngledSprite.h"

#include <cassert>

#include "Sprite.h"
#include "Utilities.h"

AngledSprite::AngledSprite(): sprite_(new Sprite())
{
    underlyingItem_ = sprite_;
}

/// Adds the specified pixmap as a frame to the specified animation for the specified angle.
///
/// If the animation already exists, the pixmap will simply be added as the next
/// frame in the animation. If the animation does not exist, it will be created with
/// the pixmap being its first frame.
void AngledSprite::addFrame(const QPixmap &frame, const std::string &toAnimation, int forAngle)
{
    assert(toAnimation != "");

    // if this animation doesn't exist for any angle
    if (animationToAngle_.find(toAnimation) == std::end(animationToAngle_))
        animationToAngle_[toAnimation] = std::vector<int>();

    // if this animation doesn't exist just for this angle
    std::vector<int> angles = animationToAngle_[toAnimation];
    if (std::find(std::begin(angles),std::end(angles),forAngle) == std::end(angles)){
        animationToAngle_[toAnimation].push_back(forAngle);
    }

    sprite_->addFrame(frame,toAnimation + std::to_string(forAngle));
}

/// Adds an animation to the AngledSprite.
/// @param angle The facing angle that frames of this animation are facing.
/// @param animationName What to call the animation.
/// @param fromSpriteSheet The SpriteSheet to create frames for the animation.
/// @param fromNode Where on the SpriteSheet to start adding frames for the animation.
/// @param toNode Where on the SpriteSheet to stop adding frames for the animation.
void AngledSprite::addAnimation(int angle, std::string animationName, const SpriteSheet &fromSpriteSheet, const Node &fromNode, const Node &toNode)
{
    if (animationToAngle_.find(animationName) == animationToAngle_.end())
        animationToAngle_[animationName] = std::vector<int>();

    animationToAngle_[animationName].push_back(angle);

    sprite_->addFrames(fromNode,toNode,fromSpriteSheet,animationName+std::to_string(angle));
}

double AngledSprite::actualFacingAngle() const
{
    return actualFacingAngle_;
}

QRectF AngledSprite::boundingBox() const
{
    return sprite_->boundingRect();
}

bool AngledSprite::hasAnimation(const std::string &animationName) const
{
    bool contains = animationToAngle_.find(animationName) != std::end(animationToAngle_);
    return contains;
}

void AngledSprite::play_(const std::string &animationName, int numTimesToPlay, int fpsToPlayAt)
{
    // approach:
    // - find animation with specified name
    // - find angle we have for that animation that is closest to current facing angle
    // - play animation (name + angle)

    assert(hasAnimation(animationName));

    // find closest angle available for specified animation and play that angle version
    std::vector<int> anglesForAnim = animationToAngle_[animationName];
    double fAngle = facingAngle();
    int closest = closestAngle(anglesForAnim,fAngle);
    sprite_->play(animationName + std::to_string(closest),numTimesToPlay,fpsToPlayAt);

    actualFacingAngle_ = closest; // keep track of what the actual angle of the animation is
}

void AngledSprite::stop_()
{
    sprite_->stop();
}

QPixmap AngledSprite::currentlyDisplayedFrame() const
{
    return sprite_->currentFrame();
}

void AngledSprite::setFacingAngle_(double angle)
{
    // approach:
    // - if currently playing an animation, pick version closest to angle

    if (sprite_->playingAnimation() != ""){
        play(playingAnimation_,sprite_->playingAnimationTimesLeftToPlay(),sprite_->playingAnimationFPS());
    }
}
