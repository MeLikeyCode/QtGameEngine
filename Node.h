#ifndef NODE_H
#define NODE_H

#include <vector>

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
class MyNode{
public:
    // constructors
    MyNode();
    MyNode( int x,  int y); // every node needs an x and y coordinate - it is what defines them!

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

// non member comparison operator
bool operator==(const MyNode& lhs, const MyNode& rhs);

// make node hashable
namespace std {
template <> struct hash<MyNode>
{
    size_t operator()(const MyNode& node) const{
        size_t mem1Hash = hash<int>()(node.x());
        size_t mem2Hash = hash<int>()(node.y());
        return mem1Hash ^ mem2Hash;
    }
};
}

#endif // NODE_H
