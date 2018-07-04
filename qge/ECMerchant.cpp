#include "ECMerchant.h"

#include "ECGuiShower.h"
#include "ShopGui.h"

using namespace qge;

/// Constructs an ECMerchant.
/// @param seller The Entity whom we want selling items.
/// @param shopGuiPos The position (in screen space) we want the ShopGui popping up in.
ECMerchant::ECMerchant(Entity *seller, const QPoint &shopGuiPos): EntityController(seller)
{
    shopGui_ = new ShopGui();
    shopGui_->setSeller(seller->inventory());
    shopGui_->setGuiPos(shopGuiPos);

    guiShower_ = new ECGuiShower(seller, shopGui_);

    connect(guiShower_,&ECGuiShower::guiShown,this,&ECMerchant::onGuiShown_);
}

void ECMerchant::addEntityOfInterest(Entity *entity)
{
    guiShower_->addEntityOfInterest(entity);
}

void ECMerchant::setDistance(double distance)
{
    guiShower_->setDistance(distance);
}

/// This function will execute in response to the underlying guiShower_
/// emiting a guiShown.
/// @param activatingEntity The entity that caused the gui to be shown.
/// Will ensure the activating entity is the buyer.
void ECMerchant::onGuiShown_(ECGuiShower *sender, Entity *activatingEntity)
{
    shopGui_->setBuyer(activatingEntity->inventory());
}
