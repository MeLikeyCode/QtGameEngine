#include "Panel.h"
#include <QGraphicsRectItem>
#include <QBrush>

Panel::Panel():
    Gui(new QGraphicsRectItem())
{
    QGraphicsRectItem* asRI = dynamic_cast<QGraphicsRectItem*>(graphicsItem_);
    asRI->setRect(0,0,300,300);

    QBrush brush;
    brush.setColor(QColor(0,0,200,125));
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    asRI->setBrush(brush);
}

void Panel::setColor(const QColor &color)
{
    QGraphicsRectItem* asRI = dynamic_cast<QGraphicsRectItem*>(graphicsItem_);

    QBrush brush;
    brush.setColor(color);
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    asRI->setBrush(brush);
}

void Panel::setOpacity(double opacity)
{
    QGraphicsRectItem* asRI = dynamic_cast<QGraphicsRectItem*>(graphicsItem_);
    asRI->setOpacity(opacity);
}

void Panel::setWidth(double width)
{
    QGraphicsRectItem* asRI = dynamic_cast<QGraphicsRectItem*>(graphicsItem_);
    double currentHeight = asRI->rect().height();
    asRI->setRect(0,0,width,currentHeight);
}

void Panel::setHeight(double height)
{
    QGraphicsRectItem* asRI = dynamic_cast<QGraphicsRectItem*>(graphicsItem_);
    double currentWidth = asRI->rect().width();
    asRI->setRect(0,0,currentWidth,height);
}
