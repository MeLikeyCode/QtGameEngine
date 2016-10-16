#include "Panel.h"
#include <QGraphicsPixmapItem>
#include <QBrush>
#include "QtUtilities.h"
#include <QGraphicsRectItem>
#include <QPen>

Panel::Panel():
    background_(new QGraphicsPixmapItem()),
    border_(new QGraphicsRectItem()),
    backgroundPixmap_(),
    backgroundColor_(Qt::gray),
    backgroundIsPixmap_(false),
    showBackground_(true),
    borderThickness_(3),
    borderColor_(Qt::black),
    showBorder_(true),
    width_(300),
    height_(300)
{
    border_->setParentItem(background_);
    draw_();
}

QGraphicsItem *Panel::getGraphicsItem()
{
    return background_;
}

/// Sets the background color of the Panel. Remember you can put opacity information in the color.
/// @see setBackgroundPixmap(const QPixmap&) for more info.
void Panel::setBackgroundColor(const QColor &color)
{
    backgroundColor_ = color;
    backgroundIsPixmap_ = false;
    draw_();
}

/// Sets the background to be a Pixmap instead of a Color.
/// Between setBackgroundColor() and setBackgroundPixmap(), which ever is the last to be called
/// is the one that will be chosen. So if setBackgroundColor() is called last, then the
/// background of the Panel will be a color. If setBackgroundPixmap() is called last, the
/// background will be a pixmap.
void Panel::setBackgroundPixmap(const QPixmap &pixmap)
{
    backgroundPixmap_ = pixmap;
    backgroundIsPixmap_ = true;
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

void Panel::showBorder(bool tf)
{
    showBorder_ = tf;
    draw_();
}

void Panel::setBorderThickness(double thickness)
{
    borderThickness_ = thickness;
    draw_();
}

void Panel::setBorderColor(const QColor &color)
{
    borderColor_ = color;
    draw_();
}

void Panel::showBackground(bool tf)
{
    showBackground_ = tf;
    draw_();
}

int Panel::height()
{
    return height_;
}

int Panel::width()
{
    return width_;
}

/// Draws the Panel in its current state (color, size, etc...).
void Panel::draw_()
{
    // draw border if showBorder_
    border_->setRect(0,0,width_,height_);
    QPen borderPen;
    borderPen.setWidth(borderThickness_);
    borderPen.setColor(borderColor_);
    border_->setPen(borderPen);
    if (showBorder_)
        border_->setVisible(true);
    else
        border_->setVisible(false);

    // draw background if show background
    background_->setPixmap(qPixmapFromColor(QSize(width_,height_),Qt::transparent));
    if (!showBackground_)
        return;
    if (backgroundIsPixmap_){
        backgroundPixmap_ = backgroundPixmap_.scaled(width_,height_);
        background_->setPixmap(backgroundPixmap_);
    }else{   
        background_->setPixmap(qPixmapFromColor(QSize(width_,height_),backgroundColor_));
    }
}
