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
    Graph(const std::unordered_set<MyNode>& nodes, const std::unordered_set<Edge>& edges);

    // readers (getters)
    std::unordered_set<MyNode> nodes() const;
    std::unordered_set<Edge> edges() const;
    std::vector<Edge> outgoingEdges(const MyNode& from) const;
    std::vector<Edge> incomingEdges(const MyNode& to) const;
    std::vector<MyNode> outgoingNodes(const MyNode& from) const; // all ADJACENT nodes this node can go to
    std::vector<MyNode> incomingNodes(const MyNode& to) const; // all ADJACENT nodes that can come to this node
    bool contains(const MyNode& node) const;
    bool contains(const Edge& edge) const;
    std::vector<MyNode> shortestPath(const MyNode& from, const MyNode& to) const;
    Tree spt(const MyNode& source) const;

    // modifiers (setters)
    void addNode(const MyNode& node);
    void addEdge(const MyNode& from, const MyNode& to,  int weight);

private:
    // main private attributes
    std::unordered_set<MyNode> nodes_;
    std::unordered_set<Edge> edges_;

    // helper attributes
    std::unordered_set<MyNode> pickedNodes_;
    std::unordered_set<MyNode> unpickedNodes_;
    std::unordered_set<Edge> pickedEdges_;
    std::unordered_set<Edge> unpickedEdges_;
    std::unordered_map<MyNode, int> nodeWeight_;
    std::unordered_map<MyNode,Edge> updatedEdge_;

    // helper functions
    void pick(const MyNode& node);
    void pick(const Edge& edge);
    bool picked(const MyNode& node) const;
    bool picked(const Edge& edge) const;
    void setWeight(const MyNode& of,  int weight);
    int weight(const MyNode& of) const;
    Edge edge(const MyNode& from, const MyNode& to) const;
    void unpickAll();
    void initializeNodeWeights(const MyNode& source);
    bool hasUnpickedNode() const;
    MyNode lightestUnpickedNode() const;
    void pickConnetedEdge(const MyNode& of);
    std::vector<MyNode> unpickedNeighbors(const MyNode& of) const;
    void updateNeighborWeights(const MyNode& of);

};

#endif // GRAPH_H
