#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <QObject>

namespace qge{

class Map;

/// An object that can provide some sort of functionality for a Map (as EntityController provides some
/// sort of functionality for an Entity).
///
/// Example
/// -------
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// MapController* mc = new SomeConcreteMapController(map);
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class MapController: public QObject // inheriting from QObject so that clients can have QPointer to us
{
public:
    MapController(Map* controlledMap);
    Map* controlledMap();

private:
    Map* controlledMap_;
};

}

#endif // MAPCONTROLLER_H
