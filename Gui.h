#ifndef GUI_H
#define GUI_H

#include <QPointF>
#include <unordered_set>

class QGraphicsItem;
class Game;
class QGraphicsItem;
class QRectF;

/// Abstract class that Represents a GUI element in a Game.
///
/// A Gui is placed relative to its parent Gui, if it does not have a parent
/// Gui, its place relative to the top left hand corner of the screen.
///
/// Most Guis offer functions for customizing their look in some way.
/// Most Guis emit signals when they are interacted with.
/// When a parent Gui is deleted, it will delete all of its child Guis.
///@author Abdullah Aghazadah
class Gui
{
public:
    Gui();
    ~Gui();

    QPointF guiPos();
    void setGuiPos(const QPointF& guiPos);
    void setGuiRotation(double degrees);

    void setParentGui(Gui* gui);

    /// Returns the root QGraphicsItem that has the drawings for the Gui.
    /// This is the QGraphicsItem that goes in the scene when Game::addGui() is called.
    virtual QGraphicsItem* getGraphicsItem() = 0;

    /// Returns the bounding box for the Gui.
    /// The default implementation will return a bounding box that encompasses all the child Gui's bounding box.
    /// This function is virtual, so that sub classes may provide a better implementation if they wish.
    virtual QRectF getGuiBoundingBox();

protected:
    std::unordered_set<Gui*> childGuis_;
    Gui* parentGui_;

    QPointF pos_; // relative to the parent (or top left of screen if no parent)

    std::vector<QRectF> getBoundingBoxesFor_(QGraphicsItem* gi, QGraphicsItem *mapTo);
};

#endif // GUI_H
