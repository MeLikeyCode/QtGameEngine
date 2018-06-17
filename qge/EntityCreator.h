#pragma once

namespace qge{

class Entity;

/// Defines an interface for the capeability to create an entity.
/// Some of our API requires an EntityCreator that they can use to create an entity.
/// They will then do various things with the created entity. For example, MCSpawner
/// uses its EntityCreator to create an entity once in a while in a random area of a Map.
class EntityCreator
{
public:
    /// Creates an entity.
    virtual Entity* createEntity() = 0;

    virtual ~EntityCreator();
};

}
