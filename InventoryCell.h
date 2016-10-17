#ifndef INVENTORYCELL_H
#define INVENTORYCELL_H

#include <QGraphicsPixmapItem>
#include <QSize>
#include <QObject>
#include <QColor>
#include <QPixmap>

class QGraphicsSceneMouseEvent;
class Item;
class Game;
class Entity;
class QGraphicsItem;

/// Represents a Gui that visualizes and allows interaction with an Item.
/// The Item must be in an Inventory in order to be used by InventoryCell.
class InventoryCell: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    InventoryCell(Game* game, int width, int height, Item* item = nullptr, QGraphicsItem*parent=nullptr);

    void setItem(Item* item);
    Item* item();

    void setSize(int width, int height);
    void setBackgroundColor(const QColor& color);
    void setBackgroundPixmap(const QPixmap& pixmap);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void positionSelectedWhileUsingPointTargetItem(QPointF pos);
    void entitySelectedWhileUsingEntityTargetItem(Entity* ent);

private:
    QGraphicsPixmapItem* picture_;
    QColor backgroundColor_;
    bool backgroundIsPixmap_;
    QPixmap backgroundPixmap_;
    int width_;
    int height_;
    Item* item_;
    Game* game_;

    void draw_();
};

#endif // INVENTORYCELL_H
