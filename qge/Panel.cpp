#include "Panel.h"

#include "QtUtilities.h"

using namespace qge;

Panel::Panel():
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
    setAcceptHoverEvents(true);
    border_->setParentItem(this);
    draw_();
}

QGraphicsItem *Panel::getGraphicsItem()
{
    return this;
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

/// Executed when the Panel is clicked, will simply emit a signal.
void Panel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(this, event->pos(), event->button());
}

/// Executed when the mouse *first* hovers over the Panel, will simply
/// emit a signal.
/// See Panel::hoverMoveEvent(QGraphicsSceneHoverEvent*) for more info.
void Panel::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit mouseHoverStarted(this,event->pos());
}

/// Executed immediately when the mouse *stops* hovering over the Panel (i.e.
/// the mouse leaves the region of the Panel), will simply emit a signal.
/// See Panel::hoverMoveEvent(QGraphicsSceneHoverEvent*) for more info.
void Panel::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit mouseHoverEnded(this,event->pos());
}

/// Executed when the mouse is moving over the Panel, will simply emit a signal.
/// When the mouse first enters the region of the Panel, a mouseHoverStarted()
/// signal will be emitted, as the mouse continues to move around the region
/// of the Panel, mouseHoverMoved() signals will be emitted, when the mouse
/// finally leaves the region of the Panel, a mouseHoverEnded() signal will
/// be emitted.
void Panel::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    emit mouseHoverMoved(this,event->pos());
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
    setPixmap(QtUtils::qPixmapFromColor(QSize(width_,height_),Qt::transparent));
    if (!showBackground_)
        return;
    if (backgroundIsPixmap_){
        backgroundPixmap_ = backgroundPixmap_.scaled(width_,height_);
        setPixmap(backgroundPixmap_);
    }else{   
        setPixmap(QtUtils::qPixmapFromColor(QSize(width_,height_),backgroundColor_));
    }
}
