#include "MapController.h"

qge::MapController::MapController(qge::Map *controlledMap):
    controlledMap_(controlledMap)
{
    // empty
}

qge::Map *qge::MapController::controlledMap()
{
    return controlledMap_;
}
