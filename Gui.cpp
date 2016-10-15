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
