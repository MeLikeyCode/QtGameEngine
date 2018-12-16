#pragma once

#include "Vendor.h"

namespace qge{

/// Simple class that represents information about a currently playing animation.
class PlayingAnimationInfo
{
public:
    PlayingAnimationInfo();
    PlayingAnimationInfo(const std::string& animationName, int fps, int timesLeftToPlay, int currentFrame);

    std::string name() const;
    int fps() const;
    int timesLeftToPlay() const;
    int currentFrame() const;

    bool isNone() const;

private:
    std::string animationName_;
    int fps_;
    int timesLeftToPlay_;
    int currentFrame_;
};

}
