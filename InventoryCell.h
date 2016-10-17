#ifndef INVENTORYCELL_H
#define INVENTORYCELL_H

#include <QGraphicsPixmapItem>
#include <QSize>
#include <QObject>
#include <QColor>
#include <QPixmap>
#include "Gui.h"

class Panel;
class QGraphicsSceneMouseEvent;
class Item;
class Game;
class Entity;
class QGraphicsItem;

/// Represents a Gui that visualizes and allows interaction with an Item.
/// The Item must be in an Inventory in order to be used by InventoryCell.
class InventoryCell: public QObject, public Gui
{
    Q_OBJECT
public:
    InventoryCell(Game* game, int width, int height, Item* item = nullptr);

    void setItem(Item* item);
    Item* item();

    void setSize(int width, int height);
    void setBackgroundColor(const QColor& color);
    void setBackgroundPixmap(const QPixmap& pixmap);

    QGraphicsItem* getGraphicsItem();

public slots:
    void onClicked_(Panel *panel, QPointF pos, int button);
    void onPositionSelectedWhileUsingPointTargetItem(QPointF pos);
    void onEntitySelectedWhileUsingEntityTargetItem(Entity* ent);

private:
    QGraphicsPixmapItem* picture_;
    Panel* background_;
    Item* item_;
    Game* game_;

    void draw_();
};

#endif // INVENTORYCELL_H
