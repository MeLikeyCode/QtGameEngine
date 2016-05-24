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
    Edge(const Node& from, const Node& to, int weight);

    // readers ("getters")
    Node from() const;
    Node to() const;
    int weight() const;

private:
    // main private attributes
    int weight_;
    Node from_;
    Node to_;
};

// comparison operator
bool operator==(const Edge& lhs, const Edge& rhs);

// Make Edge hashable
namespace std {
template <>
struct hash<Edge> // full template specialization
{
    size_t operator()(const Edge& edge) const{
        return hash<Node>()(edge.from()) ^ hash<Node>()(edge.to());
    }
};
}

#endif // EDGE_H
