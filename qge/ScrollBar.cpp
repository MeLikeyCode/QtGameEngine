#include "ScrollBar.h"

#include "QtUtilities.h"

using namespace qge;

/// Constructs a default sized ScrollBar.
ScrollBar::ScrollBar():
    bgBarWidth_(20),
    bgBarLength_(250),
    fgBarWidth_(15),
    fgBarLength_(35),
    bgBarIsColor_(true),
    fgBarIsColor_(true),
    bgBarColor_(Qt::darkGray),
    fgBarColor_(Qt::lightGray),
    fgBarPosition_(0),
    fgBar_(new QGraphicsPixmapItem(this)),
    showEvenIfFull_(false)
{
    draw_();
}

QGraphicsItem *ScrollBar::getGraphicsItem()
{
    return this;
}

/// Sets the width (i.e. thickness) of the "background bar" in pixels.
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

/// Sets the width (i.e. thickness) of the "foreground bar" in pixels.
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

/// Sets the length of the "foreground bar" to be a certain fraction of
/// the background bar length. The position of the bar may be moved in order
/// to ensure the newly sized bar fits.
void ScrollBar::setFgBarLengthAsFractionOfBgBarLength(double fraction)
{
    assert(fraction <= 1);

    fgBarLength_ = fraction * bgBarLength_;

    // clamp position if size too big
    if (fgBarIsTooHigh_()){
        fgBarPosition_ = (fgBarLength_ / 2) / bgBarLength_;
    }
    if (fgBarIsTooLow_()){
        fgBarPosition_ = (bgBarLength_ - (fgBarLength_ / 2)) / bgBarLength_;
    }

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

/// If true is passed in, the ScrollBar will be drawn even if its full.
/// If false is passed in, the ScrollBar will NOT be drawn if the bar is full.
/// By the bar being full I mean when the length of the foreground and background
/// bar are equivalent.
void ScrollBar::showEvenIfFull(bool tf)
{
    showEvenIfFull_ = tf;
    draw_();
}

/// Returns the position of the center of the "foreground bar." The position
/// is 0 to 1, 0 being the top of the background bar, 1 being the very bottom.
/// Note that the position of the *center* of the foreground bar will not be
/// exactly 0 or 1 unless the foreground bar's length is 0 (because if the
/// foreground bar has any length, the *center* will never reach the very top
/// or bottom of the background bar). I hope that makes sense! :)
double ScrollBar::fgBarCenterPos()
{
    return fgBarPosition_;
}

/// Returns the position of the top of the "foreground bar." The position
/// is 0 to 1, 0 being the top of the background bar, 1 being the very bottom.
/// Note that the position of the *top* of the foreground bar can not be exactly
/// 1 unless the length of the foreground bar is 0 (because the *top* of the
/// foreground bar will never touch the very bottom of the background bar, unless
/// the foreground bar's length is 0).
double ScrollBar::fgBarTopPos()
{
    double fgAbsCenter = fgBarPosition_ * bgBarLength_;
    double fgAbsTop = fgAbsCenter - fgBarLength_ / 2;
    double fgFracTop = fgAbsTop / bgBarLength_;

    return fgFracTop;
}

/// Returns the position of the bottom of the "foreground bar."
/// See ScrollBar::fgBarCenterPos() and ScrollBar::fgBarTopPos() for more information.
double ScrollBar::fgBarBottomPos()
{
    double fgAbsCenter = fgBarPosition_ * bgBarLength_;
    double fgAbsBot = fgAbsCenter + fgBarLength_ / 2;
    double fgFracBot = fgAbsBot / bgBarLength_;

    return fgFracBot;
}

/// Returns the width (thickness) of the foreground bar.
double ScrollBar::fgBarWidth()
{
    return fgBarWidth_;
}

/// Returns the length of the foreground bar.
double ScrollBar::fgBarLength()
{
    return fgBarLength_;
}

/// Returns the width (thickness) of the background bar.
double ScrollBar::bgBarWidth()
{
    return bgBarWidth_;
}

/// Returns the length of the background bar.
double ScrollBar::bgBarLength()
{
    return bgBarLength_;
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
    // approach:
    // -get pos of mouse from 0 - 1
    // -set that as bar position
    // -clam according to size

    if (fgBarIsBeingDragged_){
        fgBarPosition_ = event->pos().y() / bgBarLength_;

        // clamp fgBarPosition_
        if (fgBarIsTooHigh_())
            fgBarPosition_ = (fgBarLength_ / 2) / bgBarLength_;
        if (fgBarIsTooLow_())
            fgBarPosition_ = (bgBarLength_ - (fgBarLength_ / 2)) / bgBarLength_;

        draw_();
        emit positionChanged(fgBarPosition_);
    }
}

/// Draws the ScrollBar in its current state.
void ScrollBar::draw_()
{
    // approach:
    // - draw the background bar
    // - draw the foreground bar

    // if the bar is full, make it 100% transparent (i.e. "don't show it")
    // (if that flag is set)
    const double EPSILON = 3;
    if (qAbs(fgBarLength_ - bgBarLength_) < EPSILON && !showEvenIfFull_){
        QColor transparent(Qt::transparent);
        setPixmap(QtUtils::qPixmapFromColor(QSize(bgBarWidth_,bgBarLength_),transparent));
        fgBar_->setPixmap(QtUtils::qPixmapFromColor(QSize(fgBarWidth_,fgBarLength_),transparent));
        return;
    }

    // draw the background bar
    if (bgBarIsColor_)
        setPixmap(QtUtils::qPixmapFromColor(QSize(bgBarWidth_,bgBarLength_),bgBarColor_));
    else
        setPixmap(bgBarPixmap_);

    // draw the foreground bar
    // size/color
    if (fgBarIsColor_)
        fgBar_->setPixmap(QtUtils::qPixmapFromColor(QSize(fgBarWidth_,fgBarLength_),fgBarColor_));
    else
        fgBar_->setPixmap(fgBarPixmap_);
    // position foreground bar
    double fgBarX = (bgBarWidth_ - fgBarWidth_) / 2;
    double fgBarY = fgBarPosition_ * bgBarLength_ - fgBarLength_ / 2;
    fgBar_->setPos(fgBarX,fgBarY);
}

/// Returns true if according to the current fgBarPosition_ and fgBarLength_,
/// the foreground bar is sticking out of the top.
bool ScrollBar::fgBarIsTooHigh_()
{
    return fgBarPosition_ * bgBarLength_ - fgBarLength_ / 2 < 0;
}

/// Returns true if according to the current fgBarPosition_ and fgBarLength_,
/// the foreground bar is sticking out of the bottom.
bool ScrollBar::fgBarIsTooLow_()
{
    return fgBarPosition_ * bgBarLength_ + fgBarLength_ / 2 > bgBarLength_;
}
