#pragma once

#include "EntityController.h"

namespace qge{

class ECGuiShower;
class ShopGui;

/// Adds merchant behavior to an entity.
///
/// When an 'entity of interest' gets near the controlled entity, a ShopGui will pop up.
/// Use addEntityOfInterest() to add an entity that will trigger the shop gu.
class ECMerchant : public EntityController
{
    Q_OBJECT
public:
    ECMerchant(Entity* seller);

    void addEntityOfInterest(Entity* entity);
    void setDistance(double distance);

public slots:
    void onGuiShown_(ECGuiShower* sender, Entity* activatingEntity);

private:
    ECGuiShower* guiShower_;
    ShopGui* shopGui_;
};

}
