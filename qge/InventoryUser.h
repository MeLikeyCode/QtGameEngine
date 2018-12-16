#pragma once

#include "Vendor.h"

#include "Gui.h"
#include "InventoryViewer.h"

class QGraphicsItem;

namespace qge{

class InventoryViewer;
class Inventory;
class Item;
class Entity;

/// A Gui that allows "using" the Items in an Inventory.
/// @author Abdullah Aghazadah
class InventoryUser : public Gui
{
    Q_OBJECT
public:
    InventoryUser(Game* game, Inventory* inventory=nullptr);

    // TODO: define/implement veineer functions for modifying look ("veineer" b/c
    // they will simply delegate to inventoryViewer_)

    QGraphicsItem* getGraphicsItem();

public slots:
    void onItemClicked(Item* item, int button);
    void onPositionSelectedWhileUsingPointTargetItem(QPointF pos);
    void onEntitySelectedWhileUsingEntityTargetItem(Entity* entity);
private:
    std::unique_ptr<InventoryViewer> inventoryViewer_;
    Game* game_;
    Item* lastItemUsed_;
};

}
