#ifndef ANGLEDSPRITE_H
#define ANGLEDSPRITE_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "EntitySprite.h"

class Sprite;
class QRectF;
class SpriteSheet;
class Node;
class QPixmap;

class AngledSprite : public EntitySprite
{
public:
    AngledSprite();

    void addFrame(const QPixmap& frame, const std::string& toAnimation, int forAngle);
    void addAnimation(int angle, std::string animationName, const SpriteSheet& fromSpriteSheet, const Node& fromNode, const Node& toNode);

    virtual double actualFacingAngle() const override;
    virtual QRectF boundingBox() const override;
    virtual bool hasAnimation(const std::string &animationName) const override;
    virtual QPixmap currentlyDisplayedFrame() const override;

private:
    // The underlying sprite will contain all animations for every angle.
    // For example if we have a walk animation at 30 degrees, its name in sprite_ will be walk30.
    // If we are facing 30 degrees and we are playing walk, we will ask sprite to play animation "walk30"
    Sprite* sprite_;

    std::unordered_map<std::string,std::vector<int>> animationToAngle_; // for each animation, what angles do they support?

    double actualFacingAngle_;

    virtual void setFacingAngle_(double angle) override;
    virtual void play_(const std::string& animationName, int numTimesToPlay, int fpsToPlayAt) override;
    virtual void stop_() override;
};

#endif // ANGLEDSPRITE_H
