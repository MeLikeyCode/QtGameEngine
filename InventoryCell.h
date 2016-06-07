#ifndef INVENTORYCELL_H
#define INVENTORYCELL_H

#include <QGraphicsPixmapItem>
#include <QSize>
#include <QObject>

class QGraphicsSceneMouseEvent;
class Item;
class Game;
class Entity;

/// Represents a QGraphicsItem that visualizes and allows interaction with an Item.
/// The Item must be in an Inventory in order to be used by InventoryCell.
class InventoryCell: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    InventoryCell(Game* game, int width, int height, Item* item = nullptr, QGraphicsItem*parent=nullptr);

    void setItem(Item* item);
    Item* item();

    void setSize(int width, int height);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void positionSelectedWhileUsingPointTargetItem(QPointF pos);
    void entitySelectedWhileUsingEntityTargetItem(Entity* ent);

private:
    int width_;
    int height_;
    Item* item_;
    Game* game_;
};

#endif // INVENTORYCELL_H
