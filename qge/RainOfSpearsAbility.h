#pragma once

#include "Vendor.h"

#include "NoTargetAbility.h"

namespace qge{

class Sound;

/// An Ability that will rain spears around the owner, damaging all enemies of the owner but not
/// the owner himself.
class RainOfSpearsAbility : public NoTargetAbility
{
    Q_OBJECT
public:
    RainOfSpearsAbility(Entity* owner=nullptr);

    void useImplementation() override;

public slots:
    void spearStep_();

private:
    int times_;
    QTimer* timer_;
    Sound* soundEffect_;
};

}
