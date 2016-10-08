#ifndef GUI_H
#define GUI_H

#include <QPointF>
#include <unordered_set>

class QGraphicsItem;
class Game;
class QGraphicsItem;

/// Abstract class that Represents a GUI element in a Game. A Gui is placed
/// relative to the to its parent Gui, if it does not have a prent Gui, its
/// place relative to the top left hand corner of the screen.
class Gui
{
public:
    QPointF guiPos();
    void setGuiPos(const QPointF& guiPos);

    void setParentGui(Gui* gui);

    /// Returns the root QGraphicsItem that has the drawings for the Gui.
    /// This is the QGraphicsItem that goes in the scene when Game::addGui() is called.
    virtual QGraphicsItem* getGraphicsItem() = 0;

protected:
    std::unordered_set<Gui*> children_;
    Gui* parent_;

    QPointF pos_;
};

#endif // GUI_H
