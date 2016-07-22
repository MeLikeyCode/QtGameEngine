#include "Gui.h"

Gui::Gui(QGraphicsItem *parent):
    graphicsItem_(parent)
{
}

QPointF Gui::viewPos()
{
    return viewPos_;
}

void Gui::setViewPos(const QPointF &pos)
{
    viewPos_ = pos;
}
