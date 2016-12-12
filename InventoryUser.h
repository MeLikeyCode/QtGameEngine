#ifndef INVENTORYUSER_H
#define INVENTORYUSER_H

#include "Gui.h"

class InventoryViewer;
class Inventory;
class Item;
class QGraphicsItem;
class Entity;

class InventoryUser : public Gui
{
    Q_OBJECT
public:
    InventoryUser(Game* game, Inventory* inventory=nullptr);

    // TODO: define/implement veineer functions for modifying look ("veineer" b/c
    // they will simply delegate to inventoryViewer_)

public slots:
    void onItemClicked(Item* item, int button);
    void onPositionSelectedWhileUsingPointTargetItem(QPointF pos);
    void onEntitySelectedWhileUsingEntityTargetItem(Entity* entity);
private:
    InventoryViewer* inventoryViewer_;
    Game* game_;
    Item* lastItemUsed_;
};

#endif // INVENTORYUSER_H
