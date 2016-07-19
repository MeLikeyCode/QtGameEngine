#include "Button.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QBrush>
#include <QFont>

Button::Button():
    Gui(new QGraphicsRectItem()),
    borderPadding_(0),
    text_("default text")
{
    // initialize
    backgroundRectItem_ = static_cast<QGraphicsRectItem*>(graphicsItem_);
    backgroundRectItem_->setRect(0,0,200,50);

    textItem_ = new QGraphicsTextItem("default text",backgroundRectItem_);

    QBrush brush;
    brush.setColor(QColor(0,0,200,200));
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    backgroundRectItem_->setBrush(brush);
}

/// Sets the amount of padding between the text and the outter rectangle.
void Button::setBorderPadding(double amount)
{
    borderPadding_ = amount;
    redraw_();
}

void Button::setFontSize(int size)
{
    fontSize_ = size;
    redraw_();
}

void Button::setText(const std::string &text)
{
    text_ = text;
    redraw_();
}

/// Redraws the background and the text using the font size and padding and such.
void Button::redraw_()
{
    delete textItem_;
    textItem_ = new QGraphicsTextItem(QString::fromStdString(text_),graphicsItem_);

    QFont font("Times", fontSize_, QFont::Bold);
    textItem_->setFont(font);

    // adjust positioning of background/text
    double fontWidth = textItem_->boundingRect().width();
    double fontHeight = textItem_->boundingRect().height();

    textItem_->setPos(borderPadding_,borderPadding_);

    backgroundRectItem_->setRect(0,0,fontWidth + borderPadding_*2,fontHeight + borderPadding_ * 2);
}
