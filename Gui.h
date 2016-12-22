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
    /// The bounding box encompasses all the art of the Gui.
    virtual QRectF getGuiBoundingBox();

protected:
    std::unordered_set<Gui*> children_;
    Gui* parent_;

    QPointF pos_;

    std::vector<QRectF> getBoundingBoxesFor_(QGraphicsItem* gi, QGraphicsItem *mapTo);
};

#endif // GUI_H
