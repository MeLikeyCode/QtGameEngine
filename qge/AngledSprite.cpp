#include "AngledSprite.h"

#include "Sprite.h"
#include "Utilities.h"
#include "STLWrappers.h"

using namespace qge;

AngledSprite::AngledSprite(): sprite_(new Sprite())
{
    underlyingItem_ = sprite_;
    connect(sprite_,&Sprite::animationFinished,this,&AngledSprite::onInternalSpriteAnimationFinished_);
    connect(sprite_,&Sprite::animationFinishedCompletely,this,&AngledSprite::onInternalSpriteAnimationCompletelyFinished_);
    connect(sprite_,&Sprite::frameSwitched,this,&AngledSprite::onInternalSpriteFrameSwitched_);
}

/// Adds a frame to the AngledSprite.
/// @param frame The frame to add.
/// @param toAnimation Name of the animation.
/// @param forAngle Angle of the frame.
/// If the animation with the specified name does not exist, it will be created.
void AngledSprite::addFrame(const QPixmap &frame, const std::string &toAnimation, int forAngle)
{
    assert(toAnimation != "");
    bool hasDigits = std::find_if(std::begin(toAnimation),std::end(toAnimation),[](char c){return isdigit(c);}) != std::end(toAnimation);
    assert(!hasDigits); // numbers not allowed in animation names

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

/// Adds frames from a SpriteSheet to the AngledSprite.
/// @param angle The facing angle that frames are facing.
/// @param animationName Name of the animation.
/// @param fromSpriteSheet The SpriteSheet to create frames for the animation.
/// @param fromNode Where on the SpriteSheet to start adding frames for the animation.
/// @param toNode Where on the SpriteSheet to stop adding frames for the animation.
/// If the animation with the specified name does not exist, it will be created.
void AngledSprite::addFrames(int angle, std::string animationName, const SpriteSheet &fromSpriteSheet, const Node &fromNode, const Node &toNode)
{
    bool hasDigits = std::find_if(std::begin(animationName),std::end(animationName),[](char c){return isdigit(c);}) != std::end(animationName);
    assert(!hasDigits); // numbers not allowed in animation names

    if (animationToAngle_.find(animationName) == animationToAngle_.end())
        animationToAngle_[animationName] = std::vector<int>();

    std::vector<int> anglesForThisAnim = animationToAngle_[animationName];
    if (!STLWrappers::contains(anglesForThisAnim,angle))
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

void AngledSprite::play(const std::string &animationName, int numTimesToPlay, int fpsToPlayAt, int startingFrameNumber)
{
    // approach:
    // - find animation with specified name
    // - find angle we have for that animation that is closest to current facing angle
    // - play animation (name + angle)

    assert(hasAnimation(animationName));

    playingAnimation_ = animationName;
    numTimesToPlay_ = numTimesToPlay;
    fpsToPlayAt_ = fpsToPlayAt;

    // find closest angle available for specified animation and play that angle version
    std::vector<int> anglesForAnim = animationToAngle_[animationName];
    double fAngle = facingAngle();
    int closest = closestAngle(anglesForAnim,fAngle);
    sprite_->play(animationName + std::to_string(closest),numTimesToPlay,fpsToPlayAt,startingFrameNumber);

    actualFacingAngle_ = closest; // keep track of what the actual angle of the animation is
}

void AngledSprite::stop()
{
    sprite_->stop();
    playingAnimation_ = "";
}

PlayingAnimationInfo AngledSprite::playingAnimation()
{
    if (playingAnimation_ == "")
        return PlayingAnimationInfo();

    int timesLeftToPlay = sprite_->playingAnimation().timesLeftToPlay();
    int currentFrame = sprite_->currentFrameNumber();
    return PlayingAnimationInfo(playingAnimation_,fpsToPlayAt_,timesLeftToPlay,currentFrame);
}

QPixmap AngledSprite::currentlyDisplayedFrame() const
{
    return sprite_->currentFrame();
}

/// Executed when the underlying sprite emits the "animationFinished" signal.
/// Will emit our own "animationFinished" signal in response.
void AngledSprite::onInternalSpriteAnimationFinished_(Sprite *sender, std::string animation)
{
    // remove number from end
    std::string animWONums;
    for (char c : animation){
        if (!isdigit(c))
            animWONums.push_back(c);
    }

    emit animationFinished(this,animWONums);
}

/// Executed when the underlying sprite emits the "animationFinishedCompletely" signal.
/// Will emit our own "animationFinishedCompletely" signal in response.
void AngledSprite::onInternalSpriteAnimationCompletelyFinished_(Sprite *sender, std::string animation)
{
    // remove number from end
    std::string animWONums;
    for (char c : animation){
        if (!isdigit(c))
            animWONums.push_back(c);
    }

    emit animationFinishedCompletely(this,animWONums);
    playingAnimation_ = "";
}

void AngledSprite::onInternalSpriteFrameSwitched_(Sprite *sender, int fromFrameNum, int toFrameNum)
{
    emit frameSwitched(this,fromFrameNum,toFrameNum);
}

void AngledSprite::setFacingAngle_(double angle)
{
    // approach:
    // - if currently playing an animation, find version closest to angle and start playing that version from current frame index

    int frameIndex = sprite_->currentFrameNumber();

    if (sprite_->playingAnimation().name() != ""){
        play(playingAnimation_,sprite_->playingAnimation().timesLeftToPlay(),sprite_->playingAnimation().fps(),frameIndex);
    }
}
