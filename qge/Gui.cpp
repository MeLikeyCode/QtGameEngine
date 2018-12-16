#include "Gui.h"

#include "Game.h"

using namespace qge;

/// Returns the position of the Gui relative to its parent (or relative to
/// top left of screen if no parent).
Gui::Gui():
    parentGui_(nullptr)
{
}

Gui::~Gui()
{
    // destroy children
    for (Gui* gui:childGuis_){
        delete gui;
    }
}

/// Returns the position of the Gui relative to its parent (or relative to
/// the top left of the screen if no parent).
QPointF Gui::guiPos()
{
    return getGraphicsItem()->pos();
}

/// Sets the position of the Gui relative to its parent (or relative to
/// top left of screen if no parent)
void Gui::setGuiPos(const QPointF &pos)
{
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

    // if gui is null but parent is already null, don't do anything
    if (gui == nullptr && parentGui_ == nullptr)
        return;

    // if gui is null but it has a parent, remove the parent
    if (gui == nullptr && parentGui_ != nullptr){
        parentGui_->childGuis_.erase(this);
        getGraphicsItem()->setParentItem(nullptr);
        getGraphicsItem()->setVisible(false);
        parentGui_ = nullptr;
        return;
    }

    // set parent to an actual gui
    getGraphicsItem()->setParentItem(gui->getGraphicsItem());
    getGraphicsItem()->setVisible(true);
    gui->childGuis_.insert(this);
    parentGui_ = gui;
}

/// Returns the Game that the Gui is in.
Game *Gui::game()
{
    return game_;
}

QRectF Gui::getGuiBoundingBox()
{
    // approach:
    // - get graphics item, then recursively find all bboxes
    // - find an encompassing bounding box

    std::vector<QRectF> bboxes = getBoundingBoxesFor_(getGraphicsItem(),
                                                      getGraphicsItem());

    QRectF initialRect = bboxes[0];
    double lowestY = initialRect.top();
    double highestY = initialRect.bottom();
    double lowestX = initialRect.left();
    double highestX = initialRect.right();

    for (QRectF rect:bboxes){
        if (rect.top() < lowestY)
            lowestY = rect.top();
        if (rect.bottom() > highestY)
            highestY = rect.bottom();
        if (rect.left() < lowestX)
            lowestX = rect.left();
        if (rect.right() > highestX)
            highestX = rect.right();
    }

    QRectF totalBox(QPointF(lowestX,lowestY),QPointF(highestX,highestY));
    totalBox.moveTopLeft(QPointF(0,0));

    return totalBox;
}

std::vector<QRectF> Gui::getBoundingBoxesFor_(QGraphicsItem* gi, QGraphicsItem* mapTo)
{
    // push gi bounding rect
    std::vector<QRectF> boxes;
    QRectF bbox = gi->boundingRect();
    bbox.moveTopLeft(gi->mapToItem(mapTo,QPointF(0,0)));
    boxes.push_back(bbox);

    // push all childrens bounding rect
    for (QGraphicsItem* child:gi->childItems()){
        std::vector<QRectF> childBBoxes = getBoundingBoxesFor_(child,mapTo);
        for (QRectF rect:childBBoxes){
            boxes.push_back(rect);
        }
    }

    return boxes;
}
