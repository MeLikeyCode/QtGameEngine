#include "PlayingAnimationInfo.h"
#include <cassert>

/// Constructs a PlayingAnimationInfo that reprsents no animation playing.
PlayingAnimationInfo::PlayingAnimationInfo():
    fps_(-1),
    timesLeftToPlay_(0),
    currentFrame_(-1)
{
    // empty
}

/// Constructs a PlayingAnimationInfo with the specified information for a playing animation.
/// @param timesLeftToPlay -1 represents an infinite number of times left to play.
PlayingAnimationInfo::PlayingAnimationInfo(const std::string &animationName, int fps, int timesLeftToPlay, int currentFrame):
    animationName_(animationName),
    fps_(fps),
    timesLeftToPlay_(timesLeftToPlay),
    currentFrame_(currentFrame)
{
    // empty
}

int PlayingAnimationInfo::fps() const
{
    assert(!isNone());
    return fps_;
}

int PlayingAnimationInfo::timesLeftToPlay() const
{
    assert(!isNone());
    return timesLeftToPlay_;
}

int PlayingAnimationInfo::currentFrame() const
{
    assert(!isNone());
    return currentFrame_;
}

/// Returns true if this struct represents no animation playing.
bool PlayingAnimationInfo::isNone() const
{
    assert(!isNone());
    return name() == "";
}

std::string PlayingAnimationInfo::name() const
{
    return animationName_;
}
