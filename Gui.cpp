#include "Gui.h"

Gui::Gui(QGraphicsItem *graphicsItem):
    graphicsItem_(graphicsItem)
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
