#ifndef INVENTORYCELL_H
#define INVENTORYCELL_H

#include <QGraphicsPixmapItem>
#include <QSize>
#include <QMouseEvent>

class Item;

/// Represents a QGraphicsItem that visualizes and allows interaction with an Item
/// in an Inventory.
class InventoryCell: public QGraphicsPixmapItem
{
public:
    InventoryCell(int width, int height, Item* item = nullptr, QGraphicsItem*parent=nullptr);

    void setItem(Item* item);
    Item* item();

    void setSize(int width, int height);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    int width_;
    int height_;
    Item* item_;
};

#endif // INVENTORYCELL_H
