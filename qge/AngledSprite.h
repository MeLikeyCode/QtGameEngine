#pragma once

#include "Vendor.h"

#include "EntitySprite.h"

class QRectF;
class QPixmap;

namespace qge{

class Sprite;
class SpriteSheet;
class Node;


class AngledSprite : public EntitySprite
{
    Q_OBJECT
public:
    AngledSprite();

    void addFrame(const QPixmap& frame, const std::string& toAnimation, int forAngle);
    void addFrames(int angle, std::string animationName, const SpriteSheet& fromSpriteSheet, const Node& fromNode, const Node& toNode);

    virtual double actualFacingAngle() const override;
    virtual QRectF boundingBox() const override;
    virtual bool hasAnimation(const std::string &animationName) const override;
    virtual QPixmap currentlyDisplayedFrame() const override;
    virtual void play(const std::string& animationName, int numTimesToPlay, int fpsToPlayAt, int startingFrameNumber) override;
    virtual void stop() override;
    virtual PlayingAnimationInfo playingAnimation() override;

public slots:
    void onInternalSpriteAnimationFinished_(Sprite* sender, std::string animation);
    void onInternalSpriteAnimationCompletelyFinished_(Sprite* sender, std::string animation);
    void onInternalSpriteFrameSwitched_(Sprite* sender, int fromFrameNum, int toFrameNum);

private:
    // The underlying sprite will contain all animations for every angle.
    // For example if we have a walk animation at 30 degrees, its name in sprite_ will be walk30.
    // If we are facing 30 degrees and we are playing walk, we will ask sprite to play animation "walk30"
    Sprite* sprite_;

    std::string playingAnimation_;
    int numTimesToPlay_;
    int fpsToPlayAt_;

    std::unordered_map<std::string,std::vector<int>> animationToAngle_; // for each animation, what angles do they support?

    double actualFacingAngle_;

    virtual void setFacingAngle_(double angle) override;

};

}
