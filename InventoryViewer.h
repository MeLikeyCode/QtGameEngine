#ifndef INVENTORYVIEWER_H
#define INVENTORYVIEWER_H

#include "QGraphicsRectItem"

class Inventory;
class QPointF;
class InventoryCell;
class Item;

/// Represents a GUI element that visualizes and allows interaction with
/// an Inventory.
class InventoryViewer : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    InventoryViewer(int width, int height, Inventory* inventory=nullptr);

    void setInventory(Inventory* inventory);
    void setViewPos(QPointF p);
    QPointF viewPos();

public slots:
    void onItemAddedOrRemovedFromInventory(Item* item);

private:
    QPointF pos_; // the position of the InventoryViewer in view coordinates
    int width_;
    int height_;
    Inventory* inventory_;
    std::vector<InventoryCell*> cells_;
};

#endif // INVENTORYVIEWER_H
