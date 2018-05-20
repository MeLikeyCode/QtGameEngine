#include "ItemRainOfSpears.h"

#include "RainOfSpearsAbility.h"

using namespace qge;

ItemRainOfSpears::ItemRainOfSpears()
{
    setDescription("Rains spears in the vicinity. The spears damage enemies.");
    setAbility(new RainOfSpearsAbility());
}

void ItemRainOfSpears::use_()
{
    // do nothing (when item has an ability (which this one always does), when
    // item is used, it will use ability)
}
