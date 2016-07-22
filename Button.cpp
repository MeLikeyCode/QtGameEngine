#include "Button.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QBrush>
#include <QFont>

Button::Button():
    Gui(this),
    borderPadding_(0),
    text_("default text")
{
    // initialize
    setRect(0,0,200,50);

    textItem_ = new QGraphicsTextItem("default text",this);

    QBrush brush;
    brush.setColor(QColor(0,0,200,200));
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    setBrush(brush);
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

/// Executed when the user clicks on the Button. Will emit clicked() signal.
void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
}

/// Redraws the background and the text using the font size and padding and such.
void Button::redraw_()
{
    delete textItem_;
    textItem_ = new QGraphicsTextItem(QString::fromStdString(text_),this);

    QFont font("Times", fontSize_, QFont::Bold);
    textItem_->setFont(font);

    // adjust positioning of background/text
    double fontWidth = textItem_->boundingRect().width();
    double fontHeight = textItem_->boundingRect().height();

    textItem_->setPos(borderPadding_,borderPadding_);

    setRect(0,0,fontWidth + borderPadding_*2,fontHeight + borderPadding_ * 2);
}
