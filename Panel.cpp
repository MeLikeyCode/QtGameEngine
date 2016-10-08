#include "Panel.h"
#include <QGraphicsPixmapItem>
#include <QBrush>

Panel::Panel():
    pixmap_(),
    color_(Qt::gray),
    isPixmap_(false),
    width_(300),
    height_(300)
{
    pixmapItem_ = dynamic_cast<QGraphicsPixmapItem*>(graphicsItem_);
    draw_();
}

QGraphicsItem *Panel::getGraphicsItem()
{
    return pixmapItem_;
}

/// Sets the Color of the Panel. Remember you can put opacity information in the color.
/// @see setPixmap(const QPixmap&) for more info.
void Panel::setColor(const QColor &color)
{
    color_ = color;
    isPixmap_ = false;
    draw_();
}

/// Sets the background to be a Pixmap instead of a Color.
/// Between setColor() and setPixmap(), which ever is the last to be called
/// is the one that will be chosen. So if setColor() is called last, then the
/// background of the Panel will be a color. If setPixmap() is called last, the
/// background will be a pixmap.
void Panel::setPixmap(const QPixmap &pixmap)
{
    pixmap_ = pixmap;
    isPixmap_ = true;
    draw_();
}

void Panel::setWidth(double width)
{
    width_ = width;
    draw_();
}

void Panel::setHeight(double height)
{
    height_ = height;
    draw_();
}

/// Draws the Panel in its current state (color, size, etc...).
void Panel::draw_()
{
    if (isPixmap_){
        pixmap_ = pixmap_.scaled(width_,height_);
        pixmapItem_->setPixmap(pixmap_);
    }else{
        QImage img(QSize(width_,height_),QImage::Format_RGB32);
        img.fill(color_);
        pixmapItem_->setPixmap(QPixmap::fromImage(img));
    }
}
