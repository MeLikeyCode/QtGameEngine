#pragma once

#include "Vendor.h"

#include "Gui.h"
#include "InventoryViewer.h"

class QGraphicsItem;

namespace qge{

class Inventory;
class Panel;
class Label;
class Button;
class Item;
class ShopGuiBehavior;

/// A Gui that allows you to transfer Items visually from a "seller" Inventory
/// to a "buyer" Inventory.
/// To set the seller, call setSeller(Inventory*);
/// To set the buyer, call setBuyer(Inventory*);
/// The seller Inventory will be displayed. Clicking on items results in the item's
/// description being displayed. Clicking on "buy" will result in the selected
/// item being transfered from the seller to the buyer, if the behavior approves.
class ShopGui : public Gui
{
    Q_OBJECT
public:
    ShopGui();

    void setBuyer(Inventory* buyer);
    void setSeller(Inventory* seller);
    Inventory* buyer();
    Inventory* seller();

    QGraphicsItem* getGraphicsItem();

public slots:
    void onItemClicked(Item* clickedItem, int mouseButton);
    void onBuyButtonClicked();
    void onCloseButtonClicked();

private:
    Inventory* buyer_;  // the Inventory that is buying
    Item* selecteditem_;
    ShopGuiBehavior* shopBehavior_; // the rules of purchasing

    // sub guis
    std::unique_ptr<InventoryViewer> inventoryViewer_;
    Panel* panel_;  // houses the descriptionLabel_ and priceLabel_
    Label* descriptionLabel_;
    Label* priceLabel_;
    Button* buyButton_;
    Button* closeButton_;
};

}
