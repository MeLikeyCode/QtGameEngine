#include "ScrollWindow.h"

#include "ScrollBar.h"
#include "Panel.h"

using namespace qge;

ScrollWindow::ScrollWindow(): ScrollWindow(300,300)
{

}

ScrollWindow::ScrollWindow(double width, double height):
    width_(width),
    height_(height),
    verticalScrollBar_(new ScrollBar()),
    horizontalScrollBar_(new ScrollBar()),
    background_(new Panel())
{
    horizontalScrollBar_->setParentGui(verticalScrollBar_.get());
    background_->setParentGui(verticalScrollBar_.get());

    // get notified whenever the scroll bar's position's change
    connect(verticalScrollBar_.get(),&ScrollBar::positionChanged,this,&ScrollWindow::verticalOrHorizontalScrollBarPositionChanged_);
    connect(horizontalScrollBar_,&ScrollBar::positionChanged,this,&ScrollWindow::verticalOrHorizontalScrollBarPositionChanged_);

    draw_(); // draw the initial scrollbar
}

/// Adds the specified Gui to the ScrollWindow at the specified position.
/// The ScrollWindow takes ownership of the added gui (i.e. when the ScrollWindow is
/// deleted, so are the added Guis).
/// Does nothing if the Gui is already added.
void ScrollWindow::add(Gui *gui, QPointF atPos)
{
    if (contains(gui)){
        return;
    }

    gui->setParentGui(background_);
    guiToPos_[gui] = atPos;
    draw_(); // redraw the scroll bar
}

/// Removes the specified Gui from the ScrollWindow. If the Gui is not in
/// the ScrollWindow in the first place, throws assertion error.
void ScrollWindow::remove(Gui *gui)
{
    assert(contains(gui));

    gui->setParentGui(nullptr);
    guiToPos_.erase(gui);
    draw_();
}

/// Removes all Guis that have been added to the ScrollWindow.
void ScrollWindow::removeAll()
{
    // set parent of all guis to nullptr
    for (std::pair<Gui*,QPointF> guiPoint:guiToPos_){
        Gui* gui = guiPoint.first;
        gui->setParentGui(nullptr);
    }

    // clear guis
    guiToPos_.clear();

    // redraw
    draw_();
}

/// Returns true if the ScrollWindow contains the specified Gui.
bool ScrollWindow::contains(Gui *gui)
{
    return guiToPos_.find(gui) != guiToPos_.end();
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

/// Determines weather a border is shown around the ScrollWindow.
/// You can control the thickness of the border via ScrollWindow::setBorderThickness(double).
void ScrollWindow::showBorder(bool tf)
{
    background_->showBorder(tf);
}

/// Sets the border to be of the specified Color.
void ScrollWindow::setBorderColor(const QColor &color)
{
    background_->setBorderColor(color);
}

/// Sets the thickness of the border of the ScrollWindow in pixels.
void ScrollWindow::setBorderThickness(double thickness)
{
    background_->setBorderThickness(thickness);
}

/// Determines weather the background of the ScrollWindow is shown or not.
void ScrollWindow::showBackground(bool tf)
{
    background_->showBackground(tf);
}

/// Sets the background of the ScrollWindow to be the specified color.
/// If you want a picture instead, use ScrollWindow::setBackgroundPixmap(QPixmap).
void ScrollWindow::setBackgroundColor(const QColor &color)
{
    background_->setBackgroundColor(color);
}

/// Sets the background of the ScrollWindow to be the specified pixmap.
/// If you want a color instead, use ScrollWindow::setBackgroundColor(QColor).
void ScrollWindow::setBackgroundPixmap(const QPixmap &pixmap)
{
    background_->setBackgroundPixmap(pixmap);
}

QGraphicsItem *ScrollWindow::getGraphicsItem()
{
    return verticalScrollBar_.get();
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
    // relative to top left of view bounding box
    // - for the guis in view bounding box, set their visibility to true
    //  (all others to false initially), and draw them relative to this gui, but
    // with shift vector applied

    // draw the view border/background
    double windowX = verticalScrollBar_->bgBarWidth();
    double windowY = 0;
    background_->setGuiPos(QPointF(windowX,windowY));
    background_->setWidth(width_);
    background_->setHeight(height_);

    // set all guis invisible
    for (std::pair<Gui*,QPointF> guiPoint:guiToPos_){
        Gui* gui = guiPoint.first;
        gui->getGraphicsItem()->setVisible(false);
    }

    // calculate total height and width
    double totalBot = 0;
    double totalRight = 0;
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

    // draw scroll bars properly
    verticalScrollBar_->setBgBarLength(height_);
    horizontalScrollBar_->setBgBarLength(width_);
    horizontalScrollBar_->setGuiPos(QPointF(20,height_+20));
    horizontalScrollBar_->setRotation(-90);
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
