#pragma once

#include "EntityController.h"

class ECGuiShower;
class ShopGui;

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
