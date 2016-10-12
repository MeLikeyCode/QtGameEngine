#include "ScrollBar.h"
#include "QtUtilities.h"
#include <QGraphicsSceneMouseEvent>

ScrollBar::ScrollBar():
    bgBarWidth_(20),
    bgBarLength_(250),
    fgBarWidth_(15),
    fgBarLength_(35),
    bgBarIsColor_(true),
    fgBarIsColor_(true),
    bgBarColor_(Qt::blue),
    fgBarColor_(Qt::red),
    fgBarPosition_(0),
    fgBar_(new QGraphicsPixmapItem(this))
{
    draw_();
}

QGraphicsItem *ScrollBar::getGraphicsItem()
{
    return this;
}

/// Sets the width of the "background bar" in pixels.
/// The "background bar" is the strip along which the little scroll bar ("foreground bar")
/// moves about.
void ScrollBar::setBgBarWidth(double width)
{
    bgBarWidth_ = width;
    draw_();
}

/// Sets the length of the "background bar" in pixels.
/// See ScrollBar::setBgBarWidth(double) for more info.
void ScrollBar::setBgBarLength(double length)
{
    bgBarLength_ = length;
    draw_();
}

/// Sets the width of the "foreground bar" in pixels.
/// The "foreground bar" is the little scroll bar that you can drag on the "bacground bar".
/// See ScrollBar::setBgBarWidth(double) for more info.
void ScrollBar::setFgBarWidth(double width)
{
    fgBarWidth_ = width;
    draw_();
}

/// Sets the length of the "foreground bar" in pixels.
/// See ScrollBar::setFgBarWidth(double) for more info.
void ScrollBar::setFgBarLength(double length)
{
    fgBarLength_ = length;
    draw_();
}

/// Sets the color of the "background bar".
void ScrollBar::setBgBarColor(const QColor &color)
{
    bgBarIsColor_ = true;
    bgBarColor_ = color;
    draw_();
}

/// Sets the color of the "foreground bar".
void ScrollBar::setFgBarColor(const QColor &color)
{
    fgBarIsColor_ = true;
    fgBarColor_ = color;
    draw_();
}

/// Sets the backround of the "background bar" to be the specified pixmap.
void ScrollBar::setBgBarPixmap(const QPixmap &pixmap)
{
    bgBarIsColor_ = false;
    bgBarPixmap_ = pixmap;
    draw_();
}

/// Sets the background of the "foreground bar" to be the specified pixmap.
void ScrollBar::setFgBarPixmap(const QPixmap &pixmap)
{
    fgBarIsColor_ = false;
    fgBarPixmap_ = pixmap;
    draw_();
}

void ScrollBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    fgBarIsBeingDragged_ = true;
}

void ScrollBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    fgBarIsBeingDragged_ = false;
}

void ScrollBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (fgBarIsBeingDragged_){
        double oldFgPos = fgBarPosition_;
        fgBarPosition_ = event->pos().y() / bgBarLength_;

        // clamp fgBarPosition_ from 0 to 1
        if (fgBarPosition_ > 1)
            fgBarPosition_ = 1;
        if (fgBarPosition_ < 0)
            fgBarPosition_ = 0;

        const double EPSILON = 0.0001;
        if (qAbs(oldFgPos - fgBarPosition_) < EPSILON){
            emit positionChanged(fgBarPosition_);
        }

        draw_();
    }
}

/// Draws the ScrollBar in its current state.
void ScrollBar::draw_()
{
    // approach:
    // - draw the background bar
    // - draw the foreground bar

    // draw the background bar
    if (bgBarIsColor_)
        setPixmap(qPixmapFromColor(QSize(bgBarWidth_,bgBarLength_),bgBarColor_));
    else
        setPixmap(bgBarPixmap_);

    // draw the foreground bar
    // size/color
    if (fgBarIsColor_)
        fgBar_->setPixmap(qPixmapFromColor(QSize(fgBarWidth_,fgBarLength_),fgBarColor_));
    else
        fgBar_->setPixmap(fgBarPixmap_);
    // position foreground bar
    double fgBarX = (bgBarWidth_ - fgBarWidth_) / 2;
    double fgBarY = fgBarPosition_ * bgBarLength_ - fgBarLength_ / 2;
    // clamp position vertically
    if (fgBarPosition_ * bgBarLength_ - fgBarLength_ / 2 < 0)
        fgBarY = 0;
    if (fgBarPosition_ * bgBarLength_ + fgBarLength_ / 2 > bgBarLength_)
        fgBarY = bgBarLength_ - fgBarLength_;
    fgBar_->setPos(fgBarX,fgBarY);
}
