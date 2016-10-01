#ifndef GUI_H
#define GUI_H

#include <QPointF>
#include <unordered_set>

class QGraphicsItem;
class Game;

/// Abstract class that Represents a GUI element in a Game. A Gui is placed
/// relative to the to its parent Gui, if it does not have a prent Gui, its
/// place relative to the top left hand corner of the screen.
class Gui
{
    friend class Game;
public:
    Gui(QGraphicsItem* graphicsItem);

    QPointF guiPos();
    void setGuiPos(const QPointF& guiPos);

    void setParentGui(Gui* gui);

protected:
    QGraphicsItem* graphicsItem_;
    std::unordered_set<Gui*> children_;
    Gui* parent_;

    QPointF pos_;
};

#endif // GUI_H
