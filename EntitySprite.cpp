#include "EntitySprite.h"

#include <QRectF>
#include <QGraphicsItem>

EntitySprite::EntitySprite(): underlyingItem_(nullptr)
{

}

EntitySprite::~EntitySprite()
{
    delete underlyingItem_;
}

void EntitySprite::setFacingAngle(double angle)
{
    facingAngle_ = angle;
    setFacingAngle_(angle);
}

double EntitySprite::facingAngle() const
{
    return facingAngle_;
}

QRectF EntitySprite::boundingBox() const
{
    // Implementation Details:
    // - by default, return the bounding box of the underlying QGraphicsItem
    // - is virtual, so that sub classes can provide a better implementation if they wish

    return underlyingItem_->boundingRect();
}

void EntitySprite::scale(double scale)
{
    underlyingItem_->setScale(scale);
}

void EntitySprite::setPos(const QPointF &posRelToEntity)
{
    pos_ = posRelToEntity;
}

QPointF EntitySprite::pos() const
{
    return pos_;
}
