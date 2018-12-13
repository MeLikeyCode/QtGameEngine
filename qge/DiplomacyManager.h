#pragma once

#include <unordered_map>

namespace qge{

enum class Relationship{
    FRIEND,
    NEUTRAL,
    ENEMY,
    UNSPECIFIED, // The relationship is not specified
};

/// An object that can keep track of the relationship between groups.
/// Example
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
/// DiplomacyManager dm;
/// dm.setRelationship(1,2,Relationship::FRIEND); // make group 1 see group 2 as a friend
/// dm.setRelationship(2,1,Relationship::ENEMY);  // make group 2 see group 1 as an enemy, sho shad :(
/// dm.getRelationship(1,2); // find out how group1 views group2
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class DiplomacyManager
{
public:
    Relationship getRelationship(int group1, int group2);
    void setRelationship(int group1, int group2, Relationship relationship);

    class Hasher{
    public:
        size_t operator()(const std::pair<int,int>& pair) const{
            // TODO improve hash algorithm, also consider making a simple reusable point hash algorithm
            return std::hash<int>()(pair.first) + std::hash<int>()(pair.second);
        }
    };

private:
    std::unordered_map<std::pair<int,int>,Relationship,Hasher> pairToRelationship_;
};

}
