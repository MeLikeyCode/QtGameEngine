#pragma once

#include "EntityController.h"

namespace qge{

class ECGuiShower;
class ShopGui;

/// An EntityController that adds merchant-like behavior to an entity.
///
/// When an 'entity of interest' gets near the controlled entity, a ShopGui will pop up.
/// Use addEntityOfInterest() to add an entity that will trigger the ShopGui popping up.
/// The ShopGui will transfer items between the controlled entity and the trigger entity (an
/// entity of interest).
class ECMerchant : public EntityController
{
    Q_OBJECT
public:
    ECMerchant(Entity* seller, const QPoint& shopGuiPos = QPoint(0,0));

    void addEntityOfInterest(Entity* entity);
    void setDistance(double distance);

public slots:
    void onGuiShown_(ECGuiShower* sender, Entity* activatingEntity);

private:
    ECGuiShower* guiShower_;
    ShopGui* shopGui_;
};

}
