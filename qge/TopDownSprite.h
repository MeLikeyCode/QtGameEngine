#pragma once

#include "Vendor.h"

#include "EntitySprite.h"

namespace qge{

class Sprite;
class SpriteSheet;
class Node;

/// Represents top down graphics for an Entity as well.
/// @author Abdullah Aghazadah
/// @date 9-23-17
///
/// This class can be used to represent legit top down entities, or just a picture.
/// If you just want a picture, pass a QPixmap into the constructor.
///
/// For TopDownSprite, you only need to add frames from a "pure" top down view.
/// When the sprite is asked to change its facing angle, it will simply rotate
/// the underlying picture.
/// @see EntitySprite
class TopDownSprite : public EntitySprite
{
    Q_OBJECT
public:
    TopDownSprite();
    TopDownSprite(const QPixmap& pixmap);

    void addFrames(std::string animationName,
                      const SpriteSheet& fromSpriteSheet,
                      const Node& from,
                      const Node& to);

    void addFrames(std::string resourceFolder,
                      int numOfImages,
                      std::string imagePrefix);

    void addFrame(const QPixmap& frame, const std::string& toAnimation);

    virtual double actualFacingAngle() const override;
    virtual QRectF boundingBox() const override;
    virtual bool hasAnimation(const std::string& animationName) const override;
    virtual QPixmap currentlyDisplayedFrame() const override;
    virtual void play(const std::string &animationName, int numTimesToPlay, int fpsToPlayAt, int startingFrameNumber) override;
    virtual void stop() override;
    virtual PlayingAnimationInfo playingAnimation() override;

public slots:
    void onInternalSpriteAnimationFinished_(Sprite* sender, std::string animation);
    void onInternalSpriteAnimationCompletelyFinished_(Sprite* sender, std::string animation);
    void onInternalSpriteFrameSwitched_(Sprite* sender, int fromFrameNum, int toFrameNum);

private:
    Sprite* sprite_;

    virtual void setFacingAngle_(double angle) override;

    void commonInitialize_();
};

}
