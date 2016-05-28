#ifndef INVENTORYCELL_H
#define INVENTORYCELL_H

#include <QGraphicsRectItem>
#include <QSize>
#include <QMouseEvent>

class Item;

class InventoryCell: public QGraphicsRectItem
{
public:
    InventoryCell(QGraphicsItem*parent=nullptr);

    void setItem(Item* item);
    Item* item();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    Item* item_;
};

#endif // INVENTORYCELL_H
