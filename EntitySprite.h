#ifndef ENTITYSPRITE_H
#define ENTITYSPRITE_H

#include <string>

class QGraphicsItem;
class QRectF;
class QPixmap;

/// Represents the graphics of an Entity.
/// @author Abdullah Aghazadah
/// @date 9-23-17
///
/// Implementation Details
/// ======================
/// Entities go in Maps. A Map is really just a wrapper around a QGraphicsScene and an Entity is a
/// wrapper around a bunch of QGraphicsItems (more specifically, the EntitySprite of an Entity is
/// the wrapper around a bunch of QGraphicsItems). Therefore, when an Entity is inserted into a Map,
/// we need to insert the underlying QGraphicsItem representation of the Entity into the underlying
/// QGraphicsScene representation of the Map. Therefore, we need a way for the Map class to access
/// the underlying QGraphicsItem of the Entity's EntitySprite. Thus, every EntitySprite shall have
/// a pointer to its QGraphicsItem representation (called underlyingItem_).
///
/// How to implement concrete classes
/// ===============================
/// As mentioned above, the only thing your sub class really needs to make the 'underlyingItem_'
/// variable point to the correct QGraphicsItem. That is the QGraphicsItem that will be added/removed
/// from the QGraphicsScene of the Map as the Entity is added/removed from the Map.
///
/// @warning Again, all sub classes will need to ensure that they have properly set the 'underlyingItem_' as
/// this is what will be inserted/removed from the Map's QGraphicsScene as the Entity is added/removed
/// from the Map.
class EntitySprite
{
    friend class Map; // map needs access to the underlying QGraphicsItem for the reason mentioned in "Implementation Details" section of this class's API documentation.
public:

    /// Sets the angle that the EntitySprite should face.
    /// When you call setFacingAngle(someValue), the EntitySprite will attempt to face as close to the specified angle as possible.
    void setFacingAngle(double angle);

    /// Returns the current angle that the EntitySprite is supposed to be facing.
    double facingAngle() const;

    /// Returns the actual angle that the EntitySprite is facing.
    /// This may be different than the facingAngle() if the EntitySprite could not completely face where it was asked to face.
    virtual double actualFacingAngle() const = 0;

    /// Returns the bounding box of the EntitySprite.
    virtual QRectF boundingBox() const;

    /// Returns true if the EntitySprite has the specified animation.
    virtual bool hasAnimation(const std::string& animationName) const = 0;

    /// Plays the specified animation.
    virtual void play(const std::string& animationName, int numTimesToPlay, int fpsToPlayAt);

    /// Stops playing the currently playing animation. The frame that it was stopped on will still show.
    virtual void stop();

    /// Returns the currently displayed frame of the EntitySprite.
    virtual QPixmap currentlyDisplayedFrame() const = 0;

protected:
    QGraphicsItem* underlyingItem_; // the underlying QGraphicsItem represenation of the EntitySprite

    // Concrete implementations will need to implement this function.
    // From the perspective of sub classes, this function is executed when the facing angle
    // of the EntitySprite is asked to change. Sub classes should change the facing angle of the
    // underlying frames of the current animation.
    virtual void setFacingAngle_(double angle) = 0;

    double facingAngle_; // the angle that the EntitySprite is supposed to be facing
};

#endif // ENTITYSPRITE_H
