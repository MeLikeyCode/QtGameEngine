#include "MoveBehavior.h"

MoveBehavior::MoveBehavior(DynamicEntity *entity):
    entity_(entity)
{

}

DynamicEntity *MoveBehavior::getEntity()
{
    return entity_;
}
