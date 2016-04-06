#ifndef EDGE_H
#define EDGE_H

#include "Node.h"    

/// Represents an edge between two Nodes.
/// @author Abdullah Aghazadah
/// @date 4-21-15
///
/// An Edge is defined by the value of its starting ("from") and ending ("to")
/// Nodes. Therefore two Edges are equivalent if they both start and end at the
/// same (i.e. equivalent) Nodes.
///
/// @see Graph
class Edge{
public:
    // construtors
    Edge();
    Edge(const MyNode& from, const MyNode& to, int weight);

    // readers ("getters")
    MyNode from() const;
    MyNode to() const;
    int weight() const;

private:
    // main private attributes
    int weight_;
    MyNode from_;
    MyNode to_;
};

// comparison operator
bool operator==(const Edge& lhs, const Edge& rhs);

// Make Edge hashable
namespace std {
template <> struct hash<Edge>
{
    size_t operator()(const Edge& edge) const{
        return hash<MyNode>()(edge.from()) + hash<MyNode>()(edge.to());
    }
};
}

#endif // EDGE_H
