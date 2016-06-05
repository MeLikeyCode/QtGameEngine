#ifndef INVENTORYVIEWER_H
#define INVENTORYVIEWER_H

#include "QGraphicsRectItem"
#include <QObject>

class Inventory;
class QPointF;
class InventoryCell;
class Item;
class Game;

/// Represents a GUI element that visualizes and allows interaction with
/// an Inventory.
class InventoryViewer : public QObject
{
    Q_OBJECT
    // Game needs to access rectItem_ (rectItem_ needs to be added to Game's scene when
    // the InventoryViewer is added to the Game)
    friend class Game;
public:
    InventoryViewer(Game* game, int width, int height, Inventory* inventory=nullptr);

    void setInventory(Inventory* inventory);
    void setViewPos(QPointF p);
    QPointF viewPos();

public slots:
    void onItemAddedOrRemovedFromInventory(Item* item);

private:
    QGraphicsRectItem* rectItem_;
    QPointF pos_; // the position of the InventoryViewer in view coordinates
    int width_;
    int height_;
    Inventory* inventory_;
    std::vector<InventoryCell*> cells_;
    Game* game_;
};

#endif // INVENTORYVIEWER_H
