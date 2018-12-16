#pragma once

#include "Vendor.h"

namespace qge{

class Graph;

/// Represents a node.
/// @author Abdullah Aghazadah
/// @date 4-20-15
///
/// A Node simply represents a certain location using an x and y value. A Node
/// object is _defined_ by its x and y values. Two Nodes are equivalent if they
/// have the same x and y values. A Node object is similar to a point except
/// that a Node object is more discreet because its x and y values have to be
/// integers.
class Node{
public:
    // constructors
    Node();
    Node( int x,  int y); // every node needs an x and y coordinate - it is what defines them!

    // readers (getters)
    int x() const;
    int y() const;

    // modifiers (setters)
    void setX( int x);
    void setY( int y);

private:
    int x_;
    int y_;
};

// non member comparison operators
bool operator==(const qge::Node& lhs, const qge::Node& rhs);
bool operator!=(const qge::Node& lhs, const qge::Node& rhs);

}



// make node hashable
namespace std {
template <> struct hash<qge::Node>
{
    size_t operator()(const qge::Node& node) const{
        return hash<int>()(node.x()) + hash<int>()(node.y());
    }
};
}
