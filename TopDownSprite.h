#ifndef TOPDOWNSPRITE_H
#define TOPDOWNSPRITE_H

#include <string>

#include "EntitySprite.h"
#include "SpriteSheet.h"

class Sprite;

/// Represents top down graphics for an Entity.
/// @author Abdullah Aghazadah
/// @date 9-23-17
///
/// For TopDownSprite, you only need to add frames from a "pure" top down view.
/// When the sprite is asked to change its facing angle, it will simply rotate
/// the underlying picture.
class TopDownSprite : public EntitySprite
{
public:
    TopDownSprite();

    void addAnimation(std::string animationName,
                      const SpriteSheet& fromSpriteSheet,
                      const Node& from,
                      const Node& to);

    virtual double actualFacingAngle() const override;
    virtual QRectF boundingBox() const override;
    virtual bool hasAnimation(std::string animationName) const override;
    virtual void play(const std::string &animationName, int numTimesToPlay, int fpsToPlayAt) override;
    virtual void stop() override;
    virtual QPixmap currentlyDisplayedFrame() const override;

private:
    Sprite* sprite_;
};

#endif // TOPDOWNSPRITE_H
