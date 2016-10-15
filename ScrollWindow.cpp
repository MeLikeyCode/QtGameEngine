#include "ScrollWindow.h"
#include "ScrollBar.h"

ScrollWindow::ScrollWindow(): ScrollWindow(400,400)
{

}

ScrollWindow::ScrollWindow(double width, double height):
    width_(width),
    height_(height),
    verticalScrollBar_(new ScrollBar()),
    horizontalScrollBar_(new ScrollBar())
{
    // appropriately size vertical/horizontal scroll bars
    verticalScrollBar_->setBgBarLength(height_);
    horizontalScrollBar_->setBgBarLength(width_);
    horizontalScrollBar_->setParentGui(verticalScrollBar_);
    horizontalScrollBar_->setGuiPos(QPointF(20,height_));
    horizontalScrollBar_->setRotation(-90);

    // get notified whenever the scroll bar's position's change
    connect(verticalScrollBar_,&ScrollBar::positionChanged,this,&ScrollWindow::verticalOrHorizontalScrollBarPositionChanged_);
    connect(horizontalScrollBar_,&ScrollBar::positionChanged,this,&ScrollWindow::verticalOrHorizontalScrollBarPositionChanged_);

    draw_(); // draw the initial scrollbar
}

/// Adds the specified gui to the ScrollWindow at the specified position.
void ScrollWindow::add(Gui *gui, QPointF atPos)
{
    gui->setParentGui(this);
    guiToPos_[gui] = atPos;
    draw_(); // redraw the scroll bar
}

/// Sets the height of the window of the ScrollWindow.
void ScrollWindow::setHeight(double height)
{
    height_ = height;
    draw_();
}

/// Sets the width of the window of the ScrollWindow.
void ScrollWindow::setWidth(double width)
{
    width_ = width;
    draw_();
}

/// Returns the height of the ScrollWindow.
double ScrollWindow::height()
{
    return height_;
}

/// Returns the width of the ScrollWindow.
double ScrollWindow::width()
{
    return width_;
}

QGraphicsItem *ScrollWindow::getGraphicsItem()
{
    return verticalScrollBar_;
}

/// Executed whenever the positions' of one of the scroll bars changes.
/// Will simply redraw the ScrollWindow.
void ScrollWindow::verticalOrHorizontalScrollBarPositionChanged_(double pos)
{
    draw_();
}

/// Draws the ScrollWindow in its current state.
void ScrollWindow::draw_()
{
    // approach:
    // - look at position of the vertical and horizontal scroll bar and
    //  determine the "view bounding box"
    // - traverse through guis, see which ones are in the view bounding box
    // - for the ones in the view bounding box, remember their shift vector
    // relative to top left
    // - for the guis in view bounding box, set their visibility to true
    //  (all others to false), and draw them relative to this gui, but
    // with shift vector applied

    // if there are no guis, return
    if (guiToPos_.size() == 0)
        return;

    // set all guis invisible
    for (std::pair<Gui*,QPointF> guiPoint:guiToPos_){
        Gui* gui = guiPoint.first;
        gui->getGraphicsItem()->setVisible(false);
    }

    // calculate total height and width
    std::pair<Gui*,QPointF> initialGuiPoint = *(guiToPos_.begin());
    Gui* initialGui = initialGuiPoint.first;
    QPointF initialGuiPos = initialGuiPoint.second;
    double totalBot = initialGuiPos.y() + initialGui->getGuiBoundingBox().height();
    double totalRight = initialGuiPos.x() + initialGui->getGuiBoundingBox().width();
    for (std::pair<Gui*,QPointF> guiPoint: guiToPos_){
        Gui* gui = guiPoint.first;
        QPointF guiPos = guiPoint.second;
        double bot = guiPos.y() + gui->getGuiBoundingBox().height();
        double right = guiPos.x() + gui->getGuiBoundingBox().width();

        if (bot > totalBot)
            totalBot = bot;
        if (right > totalRight)
            totalRight = right;
    }
    double totalHeight = totalBot;
    double totalWidth = totalRight;
    if (totalHeight < height_)
        totalHeight = height_ + 1;
    if (totalWidth < width_)
        totalWidth = width_ + 1;

    // set length of scroll bars based on total height/total width and height/width (view)
    double verticalFraction = height_/totalHeight;
    double horizontalFraction = width_/totalWidth;
    verticalScrollBar_->setFgBarLengthAsFractionOfBgBarLength(verticalFraction);
    horizontalScrollBar_->setFgBarLengthAsFractionOfBgBarLength(horizontalFraction);

    // find view bounding box
    double bboxTop = verticalScrollBar_->fgBarTopPos() * totalHeight;
    double bboxBot = verticalScrollBar_->fgBarBottomPos() * totalHeight;
    double bboxLeft = horizontalScrollBar_->fgBarTopPos() * totalWidth;
    double bboxRight = horizontalScrollBar_->fgBarBottomPos() * totalWidth;
    QRectF viewBoundingBox(QPointF(bboxLeft,bboxTop),QPointF(bboxRight,bboxBot));

    // find out which guis are in the view bounding box
    guiInViewToShiftVector_.clear();
    for (std::pair<Gui*,QPointF> guiPoint:guiToPos_){
        Gui* gui = guiPoint.first;
        QPointF guiPos = guiPoint.second;
        QRectF guiBBox = gui->getGuiBoundingBox();
        guiBBox.moveTopLeft(guiPos);

        if (viewBoundingBox.contains(guiBBox)){
            QPointF gP = guiPos;
            QPointF shiftVector(gP.x() - viewBoundingBox.x(),gP.y() - viewBoundingBox.y());
            guiInViewToShiftVector_[gui] = shiftVector;
        }
    }

    for (std::pair<Gui*,QPointF> guiPoint:guiInViewToShiftVector_){
        Gui* gui = guiPoint.first;
        gui->getGraphicsItem()->setVisible(true);
        gui->setGuiPos(guiInViewToShiftVector_[gui]);
    }
}
