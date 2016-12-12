#include "Gui.h"
#include <QGraphicsItem>

/// Returns the position of the Gui relative to its parent (or relative to
/// top left of screen if no parent).
Gui::Gui(): parent_(nullptr)
{
}

QPointF Gui::guiPos()
{
    return pos_;
}

/// Sets the position of the Gui relative to its parent (or relative to
/// top left of screen if no parent)
void Gui::setGuiPos(const QPointF &pos)
{
    pos_ = pos;
    setPos(pos);
}

/// Sets the rotation of the Gui clockwise. 0 is not rotated.
void Gui::setGuiRotation(double degrees)
{
    setRotation(degrees);
}

/// Sets the Parent of this Gui to the specified Gui.
/// If nullptr is passed in, removes current parent.
void Gui::setParentGui(Gui *gui)
{
    // approach:
    // -book keep at gui level
    // -deletgate everything else to QGraphicsView

    // book keep

    // if gui is null but parent is already null, don't do anything
    if (gui == nullptr && parent_ == nullptr)
        return;

    // if gui is null but it has a parent, remove the parent
    if (gui == nullptr && parent_ != nullptr){
        parent_->children_.erase(this);
        setParentItem(nullptr);
        parent_ = nullptr;
        return;
    }

    // set parent to an actual gui
    setParentItem(gui);
    gui->children_.insert(this);
    parent_ = gui;
}

/// Returns the bounding rect of the Gui.
/// The bounding rect of a Gui is the rect that encompasses all of its children.
QRectF Gui::boundingRect() const
{
    // approach:
    // - get graphics item, then recursively find all bboxes
    // - find an encompassing bounding box

    // get encompassing bounding box for each child
    std::vector<QRectF> bboxes;
    for (QGraphicsItem* child:childItems()){
        std::vector<QRectF> childBBoxes = getBoundingBoxesFor_(child,
                                                          child);
        bboxes.insert(std::end(bboxes),std::begin(childBBoxes),std::end(childBBoxes));
    }

    // get total encompassing bounding box
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

void Gui::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // does nothing, because concrete classes use concrete QGraphicsItem that will paint themselves
}

std::vector<QRectF> Gui::getBoundingBoxesFor_(QGraphicsItem* gi, QGraphicsItem* mapTo) const
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
