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

    void addFrame()
    void addAnimation(int angle, std::string animationName, const SpriteSheet& fromSpriteSheet, const Node& fromNode, const Node& toNode);

    virtual double actualFacingAngle() const override;
    virtual QRectF boundingBox() const override;
    virtual bool hasAnimation(const std::string &animationName) const override;
    virtual QPixmap currentlyDisplayedFrame() const override;

private:
    Sprite* sprite_;
    std::unordered_map<std::string,std::vector<int>> animationToAngle_; // for each animation the angles we've added

    double actualFacingAngle_;

    virtual void setFacingAngle_(double angle) override;
    virtual void play_(const std::string& animationName, int numTimesToPlay, int fpsToPlayAt) override;
    virtual void stop_() override;
};

#endif // ANGLEDSPRITE_H
