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
    getGraphicsItem()->setPos(pos);
}

/// Sets the rotation of the Gui clockwise. 0 is not rotated.
void Gui::setGuiRotation(double degrees)
{
    getGraphicsItem()->setRotation(degrees);
}

/// Sets the Parent of this Gui to the specified Gui.
/// If nullptr is passed in, removes current parent.
void Gui::setParentGui(Gui *gui)
{
    // approach:
    // - set parent of internal graphics item and positioning/scene removal
    // is handled automatically

    getGraphicsItem()->setParentItem(gui->getGraphicsItem());
    gui->children_.insert(this);
}
