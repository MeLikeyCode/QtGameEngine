#ifndef ENTITYSPRITE_H
#define ENTITYSPRITE_H

#include <string>
#include <QSize>

class QGraphicsItem;
class QRectF;
class QPixmap;

/// Represents the graphics of an Entity.
/// @author Abdullah Aghazadah
/// @date 9-23-17
///
/// Example Use
/// ===========
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// Entity* entity;                                         // pointer to some Entity
/// EntitySprite* spr = new SomeConcreteEntitySprite();     // some concrete EntitySprite
/// entity->setSprite(spr);                                 // set the Entity's EntitySprite
///
/// spr->setFacingAngle(30); // make the sprite face 30 degrees
/// spr->play("walk",-1,30); // play "walk" animation an infinite number of times (-1) at 30 frames per second
/// spr->setFacingAngle(90); // make the sprite face 90 degrees (animation will automatically update to reflect this new angle)
/// spr->scale(2); // double the size of the EntitySprite (all the frames of its animations - current and future).
/// std::string currentAnim = spr->playingAnimation(); // find out what animation (if any) is currently playing
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
///
/// How to sub class properly
/// =========================
/// Entities go in Maps. A Map is really just a wrapper around a QGraphicsScene and an Entity is a
/// wrapper around a bunch of QGraphicsItems (more specifically, the EntitySprite of an Entity is
/// the wrapper around a bunch of QGraphicsItems). Therefore, when an Entity is inserted into a
/// Map, we need to insert the underlying QGraphicsItem representation of the Entity into the
/// underlying QGraphicsScene representation of the Map. Therefore, we need a way for the Map class
/// to access the underlying QGraphicsItem of the Entity's EntitySprite. Thus, every EntitySprite
/// shall have a pointer to its QGraphicsItem representation (called underlyingItem_). Sub classes
/// must ensure that this pointer points to the proper QGraphicsItem that visually represents that
/// concrete EntitySprite.
///
/// @warning As mentioned above, your sub class needs to make the 'underlyingItem_' variable point
/// to the correct QGraphicsItem. 'underlyingItem_' is the QGraphicsItem that will be added/removed from the
/// QGraphicsScene of the Map as the Entity is added/removed from the Map.
class EntitySprite
{
    friend class Map; // map needs access to the underlying QGraphicsItem for the reason mentioned in "Implementation Details" section of this class's API documentation.
    friend class Entity; // Entity::setPos() needs to set the position of the underlying QGraphicsItem
public:
    EntitySprite();
    virtual ~EntitySprite();

    /// Sets the angle that the EntitySprite should face.
    /// When you call setFacingAngle(someValue), the EntitySprite will attempt to face as close to the specified angle as possible.
    void setFacingAngle(double angle);

    /// Returns the current angle that the EntitySprite is supposed to be facing.
    double facingAngle() const;

    /// Returns the actual angle that the EntitySprite is facing.
    /// This may be different than the facingAngle() if the EntitySprite could not completely face where it was asked to face.
    virtual double actualFacingAngle() const = 0;

    /// Returns the bounding box of the EntitySprite in local coordinates.
    virtual QRectF boundingBox() const;

    /// Scales the EntitySprite.
    void scale(double scale);

    /// Returns true if the EntitySprite has the specified animation.
    virtual bool hasAnimation(const std::string& animationName) const = 0;

    /// Plays the specified animation.
    /// Throws (assertion error) if the specified animation does not exist.
    /// If the specified animation is already playing (at current angle), does nothing
    /// (so if you want to restart, call stop() then play() again).
    /// If any other animation is currently playing, it will be stop()ed first.
    void play(const std::string& animationName, int numTimesToPlay, int fpsToPlayAt);

    /// Returns the currently playing animation.
    /// Returns "" (empty string) if no animation is currently playing.
    std::string playingAnimation() const;

    /// Stops playing the currently playing animation. The frame that it was stopped on will still show.
    void stop();

    /// Returns the currently displayed frame of the EntitySprite.
    virtual QPixmap currentlyDisplayedFrame() const = 0;

protected:
    QGraphicsItem* underlyingItem_; // the underlying QGraphicsItem represenation of the EntitySprite

    // Concrete implementations will need to implement these protected virtual functions.

    // From the perspective of sub classes, this function is executed when the facing angle
    // of the EntitySprite is asked to change. Sub classes should change the facing angle of the
    // underlying frames of the currently playing animation (if any).
    virtual void setFacingAngle_(double angle) = 0;

    // From the perspective of sub classes, this function is executed when the EntitySprite is asked
    // to play a certain animation. Sub classes should change the graphics of 'underlyingItem_' such
    // that it "plays" the specified animation.
    virtual void play_(const std::string& animationName, int numTimesToPlay, int fpsToPlayAt) = 0;

    // From the perspective of sub classes, this function is executed when the EntitySprite is asked
    // to stop playing whatever (if any) animation it is currently playing. The sub classe should
    // ensure that this is indeed what happens.
    virtual void stop_() = 0;

    double facingAngle_; // the angle that the EntitySprite is supposed to be facing
    std::string playingAnimation_; // "" means none
};

#endif // ENTITYSPRITE_H
