#include "Gui.h"
#include <QGraphicsItem>

/// Returns the position of the Gui relative to its parent (or relative to
/// top left of screen if no parent).
QPointF Gui::guiPos()
{
    return pos_;
}

/// Sets the position of the Gui relative to its parent (or relative to
/// top left of screen if no parent)
void Gui::setGuiPos(const QPointF &pos)
{
    pos_ = pos;
    graphicsItem_->setPos(pos);
}

/// Sets the Parent of this Gui to the specified Gui.
void Gui::setParentGui(Gui *gui)
{
    // approach:
    // - set parent of internal graphics item and positioning/scene removal
    // is handled automatically

    graphicsItem_->setParentItem(gui->graphicsItem_);
    gui->children_.insert(this);
}
