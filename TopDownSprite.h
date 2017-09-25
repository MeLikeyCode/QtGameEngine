#ifndef TOPDOWNSPRITE_H
#define TOPDOWNSPRITE_H

#include <string>

#include "EntitySprite.h"

class Sprite;
class SpriteSheet;
class Node;

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
    TopDownSprite(const QPixmap& pixmap);

    void addAnimation(std::string animationName,
                      const SpriteSheet& fromSpriteSheet,
                      const Node& from,
                      const Node& to);

    void addAnimation(std::string resourceFolder, int numOfImages, std::string imagePrefix);

    virtual double actualFacingAngle() const override;
    virtual QRectF boundingBox() const override;
    virtual bool hasAnimation(const std::string& animationName) const override;
    virtual QPixmap currentlyDisplayedFrame() const override;

private:
    Sprite* sprite_;

    virtual void setFacingAngle_(double angle) override;
    virtual void play_(const std::string &animationName, int numTimesToPlay, int fpsToPlayAt) override;
    virtual void stop_() override;
};

#endif // TOPDOWNSPRITE_H