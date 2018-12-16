#pragma once

#include "Vendor.h"

class QGraphicsItem;
class QGraphicsItem;
class QRectF;

namespace qge{

class Game;

/// Abstract class that Represents a GUI element in a Game.
///
/// A Gui is placed relative to its parent Gui, if it does not have a parent
/// Gui, its place relative to the top left hand corner of the Game's screen.
///
/// Most Guis offer functions for customizing their look in some way.
/// Most Guis emit signals when they are interacted with.
/// When a parent Gui is deleted, it will delete all of its child Guis.
///@author Abdullah Aghazadah
class Gui: public QObject // inherits from QOBject b/c:
                          // 1) be able to use a QPointer<Gui>
                          // 2) most sub classes will need to inherit from QObject anyway
                          // 3) use signals and slots in the future
{
    Q_OBJECT
    friend class Game; // game needs to access Gui::game_ variable (needs to set it during Game::addGui/removeGui)
public:
    Gui();
    ~Gui();

    QPointF guiPos();
    void setGuiPos(const QPointF& guiPos);
    void setGuiRotation(double degrees);

    void setParentGui(Gui* gui);

    Game* game();

    /// Returns the root QGraphicsItem that has the drawings for the Gui.
    /// This is the QGraphicsItem that goes in the scene when Game::addGui() is called.
    virtual QGraphicsItem* getGraphicsItem() = 0;

    /// Returns the bounding box for the Gui.
    /// The default implementation will return a bounding box that encompasses all the child Gui's bounding box.
    /// This function is virtual, so that sub classes may provide a better implementation if they wish.
    virtual QRectF getGuiBoundingBox();

private:
    Gui* parentGui_;
    QPointer<Game> game_;
    std::unordered_set<Gui*> childGuis_;

    std::vector<QRectF> getBoundingBoxesFor_(QGraphicsItem* gi, QGraphicsItem *mapTo);
};

}
