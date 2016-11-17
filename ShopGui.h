#ifndef SHOPGUI_H
#define SHOPGUI_H

#include "Gui.h"
#include <QObject>

class Inventory;
class InventoryViewer;
class Panel;
class Label;
class Button;
class Item;
class QGraphicsItem;

/// A Gui that allows you to transfer Items visually from a "seller" Inventory
/// to a "buyer" Inventory.
/// To set the seller, call setSeller(Inventory*);
/// To set the buyer, call setBuyer(Inventory*);
/// The seller Inventory will be displayed. Clicking on items results in the item's
/// description and price being displayed. Clicking on buy will result in the selected
/// item being transfered from the seller to the buyer.
class ShopGui : public QObject, public Gui
{
    Q_OBJECT
public:
    ShopGui(Game* game);

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

    // sub guis
    InventoryViewer* inventoryViewer_;
    Panel* panel_;  // houses the descriptionLabel_ and priceLabel_
    Label* descriptionLabel_;
    Label* priceLabel_;
    Button* buyButton_;
    Button* closeButton_;

    Game* game_; // needed for closeButton_ to be able to remove gui from game
};

#endif // SHOPGUI_H
