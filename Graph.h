#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_set>
#include <unordered_map>
#include "Node.h"
#include "Edge.h"

class Tree;

/// Represents a graph.
/// @author Abdullah Aghazadah
/// @date 4-24-15
///
/// A Graph is a set of Nodes and Edges.
class Graph{
public:
    // constructors
    Graph();
    Graph(const std::unordered_set<Node>& nodes, const std::unordered_set<Edge>& edges);

    // readers (getters)
    std::unordered_set<Node> nodes() const;
    std::unordered_set<Edge> edges() const;
    std::vector<Edge> outgoingEdges(const Node& from) const;
    std::vector<Edge> incomingEdges(const Node& to) const;
    std::vector<Node> outgoingNodes(const Node& from) const; // all ADJACENT nodes this node can go to
    std::vector<Node> incomingNodes(const Node& to) const; // all ADJACENT nodes that can come to this node
    bool contains(const Node& node) const;
    bool contains(const Edge& edge) const;
    std::vector<Node> shortestPath(const Node& from, const Node& to) const;
    Tree spt(const Node& source) const;

    // modifiers (setters)
    void addNode(const Node& node);
    void addEdge(const Node& from, const Node& to,  int weight);

private:
    // main private attributes
    std::unordered_set<Node> nodes_;
    std::unordered_set<Edge> edges_;

    // helper attributes
    std::unordered_set<Node> pickedNodes_;
    std::unordered_set<Node> unpickedNodes_;
    std::unordered_set<Edge> pickedEdges_;
    std::unordered_set<Edge> unpickedEdges_;
    std::unordered_map<Node, int> nodeWeight_;
    std::unordered_map<Node,Edge> updatedEdge_;

    // helper functions
    void pick(const Node& node);
    void pick(const Edge& edge);
    bool picked(const Node& node) const;
    bool picked(const Edge& edge) const;
    void setWeight(const Node& of,  int weight);
    int weight(const Node& of) const;
    Edge edge(const Node& from, const Node& to) const;
    void unpickAll();
    void initializeNodeWeights(const Node& source);
    bool hasUnpickedNode() const;
    Node lightestUnpickedNode() const;
    void pickConnetedEdge(const Node& of);
    std::vector<Node> unpickedNeighbors(const Node& of) const;
    void updateNeighborWeights(const Node& of);

};

#endif // GRAPH_H
