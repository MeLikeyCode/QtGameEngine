#include "Bar.h"

using namespace qge;

Bar::Bar():
    minValue_(0),
    maxValue_(100),
    currentValue_(40),
    backgroundColor_(Qt::gray),
    foregroundColor_(Qt::blue),
    backgroundPixmap_(),
    foregroundPixmap_(),
    backgroundIsPixmap_(false),
    foregroundIsPixmap_(false),
    valuesAreInvalid_(false),
    width_(200),
    height_(10),
    verticalPadding_(2),
    horizontalPadding_(2)
{
    backgroundPixmapItem_.reset(new QGraphicsPixmapItem());
    foregroundPixmapItem_.reset(new QGraphicsPixmapItem(backgroundPixmapItem_.get()));
    draw_();
}

QGraphicsItem *Bar::getGraphicsItem()
{
    return backgroundPixmapItem_.get();
}

void Bar::setMinValue(double minValue)
{
    minValue_ = minValue;
    checkValues_();
    draw_();
}

double Bar::minValue()
{
    return minValue_;
}

void Bar::setMaxValue(double maxValue)
{
    maxValue_ = maxValue;
    checkValues_();
    draw_();
}

double Bar::maxValue()
{
    return maxValue_;
}

/// Sets the current value of the Bar. If its more than max, will be clamped down.
/// If its less than min, will be clamped up.
void Bar::setCurrentValue(double currentValue)
{
    currentValue_ = currentValue;
    checkValues_();
    draw_();
}

double Bar::currentValue()
{
    return currentValue_;
}

void Bar::setBackgroundColor(const QColor &color)
{
    backgroundColor_ = color;
    backgroundIsPixmap_ = false;
    draw_();
}

void Bar::setForegroundColor(const QColor &color)
{
    foregroundColor_ = color;
    foregroundIsPixmap_ = false;
    draw_();
}

void Bar::setBackgroundPixmap(const QPixmap &pixmap)
{
    backgroundPixmap_ = pixmap;
    backgroundIsPixmap_ = true;
    draw_();
}

void Bar::setForegroundPixmap(const QPixmap &pixmap)
{
    foregroundPixmap_ = pixmap;
    foregroundIsPixmap_ = true;
    draw_();
}

void Bar::setWidth(double width)
{
    width_ = width;
    draw_();
}

void Bar::setHeight(double height)
{
    height_ = height;
    draw_();
}

void Bar::setVerticalPadding(double verticalPadding)
{
    verticalPadding_ = verticalPadding;
    draw_();
}

void Bar::setHorizontalPadding(double horizontalPadding)
{
    horizontalPadding_ = horizontalPadding;
    draw_();
}

void Bar::checkValues_()
{
    // clamp down if needed
    if (currentValue_ > maxValue_)
        currentValue_ = maxValue_;

    // clamp up if needed
    if (currentValue_ < minValue_){
        currentValue_ = minValue_;
    }

    // see if values are invalid (won't be drawn)
    valuesAreInvalid_ = minValue_ > maxValue_;
}

void Bar::draw_()
{
    // if the values are invalid, draw emptyness
    if (valuesAreInvalid_){
        backgroundPixmapItem_->setPixmap(QPixmap());
        foregroundPixmapItem_->setPixmap(QPixmap());
        return;
    }

    // draw background
    double minMaxDiff = maxValue_ - minValue_;
    double minCurrDiff = currentValue_ - minValue_;
    double fgWidth = (width_ - 2*horizontalPadding_) * minCurrDiff / minMaxDiff;
    double fgHeight = height_ - 2*verticalPadding_;
    QPointF fgPos(horizontalPadding_,verticalPadding_);
    if (backgroundIsPixmap_){
        backgroundPixmap_ = backgroundPixmap_.scaled(width_,height_);
        foregroundPixmap_ = foregroundPixmap_.scaled(fgWidth,fgHeight);

        backgroundPixmapItem_->setPixmap(backgroundPixmap_);
        foregroundPixmapItem_->setPixmap(foregroundPixmap_);
    }else{
        QImage bgImg(QSize(width_,height_),QImage::Format_RGB32);
        bgImg.fill(backgroundColor_);
        backgroundPixmapItem_->setPixmap(QPixmap::fromImage(bgImg));

        QImage fgImg(QSize(fgWidth,fgHeight),QImage::Format_RGB32);
        fgImg.fill(foregroundColor_);
        foregroundPixmapItem_->setPixmap(QPixmap::fromImage(fgImg));
    }

    foregroundPixmapItem_->setPos(fgPos);
}
