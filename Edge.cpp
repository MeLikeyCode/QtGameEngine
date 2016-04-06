#include "Edge.h"
#include <cassert>

/// Do not use, just so Edge can be in a map.
Edge::Edge(){

}

/// Constructs an Edge from the specfieid Nodes and of the specified weight.
Edge::Edge(const MyNode &from, const MyNode &to,  int weight): from_(from), to_(to), weight_(weight){
    // make sure the from and to Edges are not the same Edge, that makes no sense
    assert(!(from == to));
}

/// Returns the starting Node of the Edge.
MyNode Edge::from() const{
    return from_;
}

/// Returns the ending Node of the Edge.
MyNode Edge::to() const{
    return to_;
}

/// Returns the weight of the Edge.
int Edge::weight() const{
    return weight_;
}

/// Returns true if the two edges have the same starting and ending Nodes, _regardeless of their weights_.
bool operator==(const Edge &lhs, const Edge &rhs){
    return (lhs.from() == rhs.from()) && (lhs.to() == rhs.to());
}
