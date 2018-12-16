#include "Button.h"

using namespace qge;

Button::Button():
    borderPadding_(4),
    text_("default text"),
    textIsBold_(false),
    textIsItalic_(false),
    textColor_(Qt::cyan),
    backgroundColor_(Qt::darkGray),
    backgroundPixmap_(":/resources/graphics/misc/buttonbg.png"),
    backgroundIsPixmap_(true),
    fontSize_(12),
    textItem_(nullptr)
{
    draw_();
}

QGraphicsItem *Button::getGraphicsItem()
{
    return this;
}

/// Sets the amount of padding between the text and the outter rectangle.
void Button::setBorderPadding(double amount)
{
    borderPadding_ = amount;
    draw_();
}

void Button::setFontSize(int size)
{
    fontSize_ = size;
    draw_();
}

void Button::setText(const std::string &text)
{
    text_ = text;
    draw_();
}

void Button::setTextColor(const QColor &color)
{
    textColor_ = color;
    draw_();
}

void Button::setTextBold(bool tf)
{
    textIsBold_ = tf;
    draw_();
}

void Button::setTextItalic(bool tf)
{
    textIsItalic_ = tf;
    draw_();
}

void Button::setBackgroundColor(const QColor &color)
{
    backgroundColor_ = color;
    backgroundIsPixmap_ = false;
    draw_();
}

void Button::setBackgroundPixmap(const QPixmap &pixmap)
{
    backgroundPixmap_ = pixmap;
    backgroundIsPixmap_ = true;
    draw_();
}

/// Returns the height of the Button.
double Button::height()
{
    return backgroundPixmap_.height();
}

/// Returns the width of the Button.
double Button::width()
{
    return backgroundPixmap_.width();
}

/// Executed when the user clicks on the Button. Will emit clicked() signal.
void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
}

/// Redraws the background and the text using the font size and padding and such.
void Button::draw_()
{
    // draw text
    if (textItem_)
        delete textItem_;

    textItem_ = new QGraphicsTextItem(QString::fromStdString(text_),this);
    textItem_->setDefaultTextColor(textColor_);
    textItem_->setPos(borderPadding_,borderPadding_);

    QFont font("Times", fontSize_);
    font.setBold(textIsBold_);
    font.setItalic(textIsItalic_);
    textItem_->setFont(font);

    // draw background
    double fontWidth = textItem_->boundingRect().width();
    double fontHeight = textItem_->boundingRect().height();
    double bgWidth = fontWidth + borderPadding_ * 2;
    double bgHeight = fontHeight + borderPadding_ * 2;

    if (backgroundIsPixmap_){
        backgroundPixmap_ = backgroundPixmap_.scaled(bgWidth,bgHeight);
        setPixmap(backgroundPixmap_);
    }else{
        QImage img(QSize(bgWidth,bgHeight),QImage::Format_RGB32);
        img.fill(backgroundColor_);
        setPixmap(QPixmap::fromImage(img));
    }
}
