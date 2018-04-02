#include "ShopGui.h"

#include "InventoryViewer.h"
#include "Panel.h"
#include "Label.h"
#include "Button.h"
#include "Inventory.h"
#include "Item.h"
#include "Game.h"
#include "ShopBehaviorNoCost.h"

using namespace qge;

ShopGui::ShopGui():
    inventoryViewer_(new InventoryViewer()),
    selecteditem_(nullptr),
    shopBehavior_(new ShopBehaviorNoCost()),
    panel_(new Panel()),
    descriptionLabel_(new Label()),
    priceLabel_(new Label()),
    buyButton_(new Button()),
    closeButton_(new Button())
{
    // set up parent/child relationship b/w sub guis
    panel_->setParentGui(inventoryViewer_.get());
    descriptionLabel_->setParentGui(panel_);
    priceLabel_->setParentGui(panel_);
    buyButton_->setParentGui(panel_);
    closeButton_->setParentGui(panel_);

    // position/size/apperence
    inventoryViewer_->setNumCellsHorizontally(5);
    double inventoryViewerWidth = inventoryViewer_->width();
    panel_->setGuiPos(QPointF(20,inventoryViewer_->height()));
    panel_->setWidth(inventoryViewerWidth);
    panel_->setHeight(150);
    descriptionLabel_->setWidth(inventoryViewerWidth);
    descriptionLabel_->setGuiPos(QPointF(10,10)); // a little inside the panel
    priceLabel_->setWidth(inventoryViewerWidth);
    priceLabel_->setGuiPos(QPointF(10,100));
    buyButton_->setText("buy");
    buyButton_->setGuiPos(QPointF(0,panel_->height()));
    closeButton_->setText("close");
    closeButton_->setGuiPos(QPointF(inventoryViewerWidth - closeButton_->width(), panel_->height()));

    // connect
    connect(inventoryViewer_.get(),&InventoryViewer::itemClicked,this,&ShopGui::onItemClicked);
    connect(buyButton_,&Button::clicked,this,&ShopGui::onBuyButtonClicked);
    connect(closeButton_,&Button::clicked,this,&ShopGui::onCloseButtonClicked);

}

void ShopGui::setBuyer(Inventory *buyer)
{
    buyer_ = buyer;
}

void ShopGui::setSeller(Inventory *seller)
{
    inventoryViewer_->setInventory(seller);
}

Inventory *ShopGui::buyer()
{
    return buyer_;
}

Inventory *ShopGui::seller()
{
    return inventoryViewer_->inventory();
}

QGraphicsItem *ShopGui::getGraphicsItem()
{
    return inventoryViewer_->getGraphicsItem();
}

/// Executed when a sellers item has been clicked.
/// Will display its description/price.
void ShopGui::onItemClicked(Item *clickedItem, int mouseButton)
{
    // if not left click, don't do anything
    if (mouseButton != Qt::LeftButton)
        return;

    selecteditem_ = clickedItem;

    // update description/price
    descriptionLabel_->setText(clickedItem->description());
    priceLabel_->setText("price: not implemented yet");
}

/// Executed when the buy button is clicked.
/// Will transfer the item from the buyer to the seller if behavior says
/// buy was succesfull.
void ShopGui::onBuyButtonClicked()
{
    Inventory* b = buyer();
    Inventory* s = seller();

    bool succesfullyBought = shopBehavior_->buy(selecteditem_, s, b);

    if (succesfullyBought){
        s->removeItem(selecteditem_);
        b->addItem(selecteditem_);
    }
}

/// Executed when the close button is clicked.
/// Will remove the ShopGui from the Game.
void ShopGui::onCloseButtonClicked()
{
    Game* g = game();
    if (g)
        g->removeGui(this);
}
