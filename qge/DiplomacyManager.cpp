#include "DiplomacyManager.h"


/// Tells you how `group1` views `group2`, either as a FRIEND, NEUTRAL, or an ENEMY.
/// @attention Does not tell you how `group2` feels towards `group1`, only how `group1` feels towards `group2`.
qge::Relationship qge::DiplomacyManager::getRelationship(int group1, int group2)
{
    auto itr = pairToRelationship_.find(std::make_pair(group1,group2));
    if (itr == std::end(pairToRelationship_))
        return Relationship::UNSPECIFIED;
    return (*itr).second;
}

/// Specifies how `group1` should feel towards `group2`.
/// @see getRelationship().
void qge::DiplomacyManager::setRelationship(int group1, int group2, qge::Relationship relationship)
{
    pairToRelationship_[std::make_pair(group1,group2)] = relationship;
}
